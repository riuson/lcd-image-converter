#ifndef ICONVERTER_H
#define ICONVERTER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class IConverter
{
public:
    virtual void loadSettings() = 0;
    virtual void saveSettings() = 0;
    virtual QString name() = 0;
    virtual QString displayName() = 0;
    enum BytesOrder
    {
        LittleEndian = 0,
        BigEndian = 1
    };
    enum DataLength
    {
        Data8 = 8,
        Data16 = 16,
        Data32 = 32
    };
};
Q_DECLARE_INTERFACE (IConverter,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // ICONVERTER_H
