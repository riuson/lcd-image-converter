#ifndef ICONVERTER_H
#define ICONVERTER_H
//-----------------------------------------------------------------------------
#include <QObject>

#include <QImage>
//-----------------------------------------------------------------------------
class BitmapData;
//-----------------------------------------------------------------------------
class IConverter
{
public:
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;
    virtual QString name() = 0;
    virtual QString displayName() = 0;
    virtual QImage preprocessImage(const QImage &source) = 0;
    virtual void processImage(const QImage &preprocessedImage, BitmapData *output) = 0;
    enum DataLength
    {
        Data8 = 8,
        Data16 = 16,
        Data32 = 32
    };
    virtual bool swapBytes() = 0;
    virtual DataLength length() = 0;
    virtual bool mirror() = 0;
    virtual bool pack() = 0;

    virtual void setSwapBytes(bool value) = 0;
    virtual void setLength(DataLength value) = 0;
    virtual void setMirror(bool value) = 0;
    virtual void setPack(bool pack) = 0;
};
Q_DECLARE_INTERFACE (IConverter,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // ICONVERTER_H
