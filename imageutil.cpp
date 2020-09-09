#include "imageutil.h"

/**
 * 图片转换为颜色集
 */
QColor ImageUtil::getImageAverageColor(QImage image, int maxSize)
{
    int hCount = image.width();
    int vCount = image.height();

    if (hCount > maxSize || vCount > maxSize) // 数量过大，按比例缩减
    {
        double prop = (double)maxSize / qMax(hCount, vCount);
        image.scaledToWidth(image.width() * prop); // 缩放到最大大小
    }

    long long sumr = 0, sumg = 0, sumb = 0;

    int w = image.width(), h = image.height();
    int sumSize = w * h;
    for (int y = 0; y < h; y++)
    {
        QRgb *line = (QRgb *)image.scanLine(y);
        for (int x = 0; x < w; x++)
        {
            int r = qRed(line[x]), g = qGreen(line[x]), b = qBlue(line[x]);
            sumr += r;
            sumg += g;
            sumb += b;
        }
    }

    sumr /= sumSize;
    sumg /= sumSize;
    sumb /= sumSize;
    return QColor(sumr, sumg, sumb);
}

/** 
 * 获取图片中所有的颜色
 */
QList<ColorOctree::ColorCount> ImageUtil::extractImageThemeColors(QImage image, int count)
{
    auto octree = ColorOctree(image, IMAGE_CALC_PIXEL_MAX_SIZE, count);
    auto result = octree.result();

    if (!result.size() || result.first().count <= 0)
        return result;

    // 过滤太少的颜色
    /*int maxCount = result.first().count;
    int minCount = maxCount / 1000; // 小于最大项1‰的都去掉
    while (result.last().count < minCount)
        result.removeLast();*/

    return result;
}

/**
 * 获取反色
 */
QColor ImageUtil::getInvertColor(QColor color)
{
    auto getInvert = [=](int c) -> int{
        if (c < 96 || c > 160)
            return 255 - c;
        else if (c < 128)
            return 255;
        else
            return 0;
    };

    color.setRed(getInvert(color.red()));
    color.setGreen(getInvert(color.green()));
    color.setBlue(getInvert(color.blue()));
    return color;
}
