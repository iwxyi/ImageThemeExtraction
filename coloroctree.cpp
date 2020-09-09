#include "coloroctree.h"

ColorOctree::ColorOctree()
{
    root = new OctreeNode();
    memset(root, 0, sizeof(OctreeNode));
    leafCount = 0;
}

ColorOctree::ColorOctree(QImage image, int maxPool) : ColorOctree()
{
    // 缩减尺寸
    int hCount = image.width();
    int vCount = image.height();
    if (hCount * vCount > maxPool) // 数量过大，按比例缩减
    {
        double prop = (double)maxPool / (hCount * vCount);
        image.scaledToWidth(image.width() * prop); // 缩放到最大大小
    }

    // 开始建树
    buildTree(image);
}

ColorOctree::~ColorOctree()
{
    if (root)
        delete root;
}

void ColorOctree::buildTree(QImage image)
{
    // 先转换为颜色，再添加到八叉树节点
    // 据说使用按行读取的方式能加快效率
    int w = image.width(), h = image.height();
    for (int y = 0; y < h; y++)
    {
        QRgb *line = (QRgb *)image.scanLine(y);
        for (int x = 0; x < w; x++)
        {
            int r = qRed(line[x]), g = qGreen(line[x]), b = qBlue(line[x]);
            RGB rgb{r, g, b};
            addColor(root, &rgb, 0);
            while (leafCount > maxCount)
                reduceTree();
        }
    }
}

QList<ColorOctree::ColorCount*> ColorOctree::result()
{
    QList<ColorCount*> counts;
    colorStats(root, &counts);
    std::sort(counts.begin(), counts.end(), [=](ColorCount *a, ColorCount *b) {
        if (a->count > b->count)
            return true;
        if (a->count < b->count)
            return false;
        return strcmp(a->color, b->color) < 0;
    });
    return counts;
}

void ColorOctree::addColor(ColorOctree::OctreeNode *node, RGB *color, int level)
{
    if (node->isLeaf)
    {
        node->pixelCount++;
        node->red += color->red;
        node->green += color->green;
        node->blue += color->blue;
    }
    else
    {
        /**
         * eg.
         *   R: 10101101
         *   G: 00101101
         *   B: 10010010
         *
         * idx: 50616616
         */
        unsigned char r = (color->red >> (7 - level)) & 1;
        unsigned char g = (color->green >> (7 - level)) & 1;
        unsigned char b = (color->blue >> (7 - level)) & 1;

        int idx = (r << 2) + (g << 1) + b;

        if (!node->children[idx])
        {
            OctreeNode *tmp = node->children[idx] = new OctreeNode;
            memset(tmp, 0, sizeof(OctreeNode));
            if (level == 7)
            {
                tmp->isLeaf = true;
                leafCount++;
            }
            else
            {
                reducible[level].push_front(tmp);
            }
        }

        addColor(node->children[idx], color, level + 1);
    }
}

void ColorOctree::reduceTree()
{
    // 找到最深的叶子
    int lv = 6;
    while (reducible[lv].empty() && lv >= 0)
        lv--;
    if (lv < 0)
        return;

    // 移除该节点
    OctreeNode *node = reducible[lv].front();
    reducible[lv].pop_front();

    // 合并子节点
    int r = 0, g = 0, b = 0;
    int count = 0;
    for (int i = 0; i < 8; i++)
    {
        if (!node->children[i])
            continue;
        r += node->children[i]->red;
        g += node->children[i]->green;
        b += node->children[i]->blue;
        count += node->children[i]->pixelCount;
        leafCount--;

        delete node->children[i];
        node->children[i] = nullptr;
    }

    node->isLeaf = true;
    node->red = r;
    node->green = g;
    node->blue = b;
    node->pixelCount = count;
    leafCount++;
}

/**
 * 获取颜色结果
 */
void ColorOctree::colorStats(ColorOctree::OctreeNode *node, QList<ColorOctree::ColorCount *> *colors)
{
    if (node->isLeaf)
    {
        int r = node->red / node->pixelCount;
        int g = node->green / node->pixelCount;
        int b = node->blue / node->pixelCount;

        ColorCount *cnt = new ColorCount;
        sprintf(cnt->color, "%.2X%.2X%.2X", r, g, b);
        cnt->count = node->pixelCount;
        cnt->colorValue = (r << 16) + (g << 8) + b;
        cnt->red = r;
        cnt->green = g;
        cnt->blue = b;

        colors->push_back(cnt);
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        if (node->children[i])
        {
            colorStats(node->children[i], colors);
        }
    }
}

