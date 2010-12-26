#ifndef CONVERTERGRAYSCALE_H
#define CONVERTERGRAYSCALE_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"
//-----------------------------------------------------------------------------
class ConverterGrayscale : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterGrayscale(QObject *parent = 0);
    ~ConverterGrayscale();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    bool swapBytes();
    DataLength length();
    bool mirror();
    bool pack();
    int depth();

    void setSwapBytes(bool value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setDepth(int value);
private:
    bool mSwapBytes;
    DataLength mDataLength;
    bool mMirrorBytes;
    bool mPack;
    int mBitsPerPoint;

    void makeGrayscale(QImage &image);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERGRAYSCALE_H
