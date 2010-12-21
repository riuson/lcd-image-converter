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

    void options(BytesOrder *orderBytes,
                 DataLength *length,
                 bool *mirror,
                 bool *pack,
                 int *bitsPerPoint);
    void setOptions(const BytesOrder &orderBytes,
                    const DataLength &length,
                    const bool mirror,
                    const bool pack,
                    const int bitsPerPoint);
private:
    BytesOrder mBytesOrder;
    DataLength mDataLength;
    bool mMirrorBytes;
    bool mPack;
    int mBitsPerPoint;
};
//-----------------------------------------------------------------------------
#endif // CONVERTERGRAYSCALE_H
