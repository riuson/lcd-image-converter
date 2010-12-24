#ifndef CONVERTERMONO_H
#define CONVERTERMONO_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"
//-----------------------------------------------------------------------------
class ConverterMono : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterMono(QObject *parent = 0);
    ~ConverterMono();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    enum ConvMonoType
    {
        Edge = 0,
        DiffuseDither = 1,
        OrderedDither = 2,
        ThresholdDither = 3
    };

    BytesOrder order();
    DataLength length();
    bool mirror();
    bool pack();
    int level();
    ConvMonoType dithType();

    void setOrder(BytesOrder value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setLevel(int value);
    void setDithType(ConvMonoType value);
private:
    BytesOrder mBytesOrder;
    DataLength mDataLength;
    bool mMirrorBytes;
    int mBlackWhiteLevel;
    ConvMonoType mDithType;
private:
    void makeMonochrome(QImage &image);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERMONO_H
