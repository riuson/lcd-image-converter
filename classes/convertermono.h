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

    void options(BytesOrder *orderBytes,
                 DataLength *length,
                 bool *mirror,
                 int *level);
    void setOptions(const BytesOrder &orderBytes,
                    const DataLength &length,
                    const bool mirror,
                    const int level);
private:
    BytesOrder mBytesOrder;
    DataLength mDataLength;
    bool mMirrorBytes;
    int mBlackWhiteLevel;
};
//-----------------------------------------------------------------------------
#endif // CONVERTERMONO_H
