#ifndef CONVERTERCOLOR_H
#define CONVERTERCOLOR_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"
//-----------------------------------------------------------------------------
class ConverterColor : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterColor(QObject *parent = 0);
    ~ConverterColor();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    enum ColorsOrder
    {
        ColorsOrderRGB = 0,
        ColorsOrderRBG = 1,
        ColorsOrderGRB = 2,
        ColorsOrderGBR = 3,
        ColorsOrderBRG = 4,
        ColorsOrderBGR = 5
    };

    void options(BytesOrder *orderBytes,
                 DataLength *length,
                 bool *mirror,
                 bool *pack,
                 int *bitsPerPointRed,
                 int *bitsPerPointGreen,
                 int *bitsPerPointBlue,
                 ColorsOrder *orderColors);
    void setOptions(const BytesOrder &orderBytes,
                    const DataLength &length,
                    const bool mirror,
                    const bool pack,
                    const int bitsPerPointRed,
                    const int bitsPerPointGreen,
                    const int bitsPerPointBlue,
                    const ColorsOrder orderColors);
private:
    BytesOrder mBytesOrder;
    DataLength mDataLength;
    bool mMirrorBytes;
    bool mPack;
    int mBitsPerPointRed;
    int mBitsPerPointGreen;
    int mBitsPerPointBlue;
    ColorsOrder mOrderColors;

    void makeGradations(QImage &image);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERCOLOR_H
