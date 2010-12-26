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
        None = 0,
        Rotate90 = 1,
        Rotate180 = 2,
        Rotate270 = 3,
        FlipHorizontal = 4,
        FlipVertical = 8,
        Inverse = 16
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
