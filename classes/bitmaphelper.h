#ifndef BITMAPHELPER_H
#define BITMAPHELPER_H
//-----------------------------------------------------------------------------
#include <QImage>
//-----------------------------------------------------------------------------
class BitmapHelper
{
public:
    enum BitmapHelperTransformCodes
    {
        Rotate90,
        Rotate180,
        Rotate270,
        FlipHorizontal,
        FlipVertical,
        Inverse
    };
    static QImage transform(BitmapHelperTransformCodes type, QImage *source);
    static QImage rotate90(QImage *source);
    static QImage rotate180(QImage *source);
    static QImage rotate270(QImage *source);
    static QImage flipHorizontal(QImage *source);
    static QImage flipVertical(QImage *source);
};
//-----------------------------------------------------------------------------
#endif // BITMAPHELPER_H
