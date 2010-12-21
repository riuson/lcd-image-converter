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

    enum ConvMonoType
    {
        Edge = 0,
        DiffuseDither = 1,
        OrderedDither = 2,
        ThresholdDither = 3
    };

    void options(BytesOrder *orderBytes,
                 DataLength *length,
                 bool *mirror,
                 int *level,
                 ConvMonoType *dithType);
    void setOptions(const BytesOrder &orderBytes,
                    const DataLength &length,
                    const bool mirror,
                    const int level,
                    const ConvMonoType dithType);
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
