#ifndef PIXMAPUTIL_H
#define PIXMAPUTIL_H

#include <QPixmap>
#include <QList>
#include "coloroctree.h"

#define IMAGE_CALC_PIXEL_MAX_COUNT 250000

class ImageUtil
{
public:
    static QColor getImageAverageColor(QImage image, int maxPool = IMAGE_CALC_PIXEL_MAX_COUNT);

    static ColorList extractImageThemeColors(QImage image, int count);
};

#endif // PIXMAPUTIL_H
