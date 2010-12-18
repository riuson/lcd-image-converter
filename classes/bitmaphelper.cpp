#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
#include <QPainter>
//-----------------------------------------------------------------------------
QImage BitmapHelper::transform(BitmapHelperTransformCodes type, QImage *source)
{
    QImage result(*source);
    switch (type)
    {
    case Rotate90:
        result = rotate90(source);
        break;
    case Rotate180:
        result = rotate180(source);
        break;
    case Rotate270:
        result = rotate270(source);
        break;
    case FlipHorizontal:
        result = flipHorizontal(source);
        break;
    case FlipVertical:
        result = flipVertical(source);
        break;
    case Inverse:
        result.invertPixels();
        break;
    }
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate90(QImage *source)
{
    QImage result = QImage(source->height(), source->width(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(result.width(), 0));
    painter.rotate(90);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate180(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(result.width(), result.height()));
    painter.rotate(180);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate270(QImage *source)
{
    QImage result = QImage(source->height(), source->width(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(0, result.height()));
    painter.rotate(270);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::flipHorizontal(QImage *source)
{
    QImage result = source->mirrored(true, false);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::flipVertical(QImage *source)
{
    QImage result = source->mirrored(false, true);
    return result;
}
//-----------------------------------------------------------------------------
