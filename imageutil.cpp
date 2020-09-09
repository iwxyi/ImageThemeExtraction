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
}

/** 
 * 获取图片中所有的颜色
 */
ColorList ImageUtil::extractImageThemeColors(QImage image, int count)
{
    auto octree = ColorOctree(image, IMAGE_CALC_PIXEL_MAX_COUNT);


    return ColorList{};
}
