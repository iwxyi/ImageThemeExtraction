#include "imageutil.h"

/**
 * 图片转换为颜色集
 */
QColor ImageUtil::getImageAverageColor(QImage image, int maxPool)
{
    int hCount = image.width();
    int vCount = image.height();

    if (hCount * vCount > maxPool) // 数量过大，按比例缩减
    {
        double prop = (double)maxPool / (hCount * vCount);
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
ColorList ImageUtil::extractImageThemeColors(QImage image, int count)
{
    auto octree = ColorOctree(image, IMAGE_CALC_PIXEL_MAX_COUNT);
    auto result = octree.result();

    // 结果转换为 QColor
    QList<QColor> colors;
    foreach (auto r, result)
    {
        qDebug() << "颜色结果：" << r->red << r->green << r->blue << r->count;
        colors.append(QColor(r->red, r->green, r->blue));
    }

    return colors;
}
