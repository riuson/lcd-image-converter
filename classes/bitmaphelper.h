#ifndef BITMAPHELPER_H
#define BITMAPHELPER_H
//-----------------------------------------------------------------------------
#include <QImage>
//-----------------------------------------------------------------------------
typedef struct
{
    int width;
    int height;
    int offsetX;
    int offsetY;
    bool center;
} tResizeInfo;
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
    static QImage resize(QImage *source, int width, int height, int offsetX, int offsetY, bool center, const QColor &backColor);
};
//-----------------------------------------------------------------------------
#endif // BITMAPHELPER_H
