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

    // 可以过滤太少的颜色，看情况开关
    int maxCount = result.first().count;
    int minCount = maxCount / 1000; // 小于最大项1‰的都去掉
    while (result.last().count < minCount)
        result.removeLast();

    return result;
}

/**
 * 获取图片中的所有主题色
 * 使用最小差值法一一求差
 * 然后和调色盘中颜色对比
 * 取出每种最是相近的颜色
 */
QList<QColor> ImageUtil::extractImageThemeColorsInPalette(QImage image, QList<QColor> paletteColors, int needCount)
{
    auto octree = ColorOctree(image, IMAGE_CALC_PIXEL_MAX_SIZE, paletteColors.size());
    auto result = octree.result();

    QList<QColor> colors;
    for (int i = 0; i < result.size(); i++)
    {
        auto cc = result.at(i);
        QColor nestColor;
        int minVariance = 255*255*3+1;
        for (int j = 0; j < paletteColors.size(); j++)
        {
            auto qc = paletteColors.at(j);

            // 差值计算：直接计算方差吧
            int dr = cc.red - qc.red();
            int dg = cc.green - qc.green();
            int db = cc.blue - qc.blue();
            int variance = dr * dr + dg * dg + db * db;

            if (minVariance > variance)
            {
                minVariance = variance;
                nestColor = qc;
            }
        }
        // 找到方差最小的颜色
        colors.append(nestColor);
    }

    return colors;
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
