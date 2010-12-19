#ifndef IDATACONTAINER_H
#define IDATACONTAINER_H
//-----------------------------------------------------------------------------
#include <QtGlobal>
class QImage;
class QString;
//-----------------------------------------------------------------------------
class IDataContainer
{
public:
    virtual QImage *image(const QString &key) = 0;
    virtual void setImage(const QString &key, QImage *image) = 0;
    virtual void transform(const QString &key, int code) = 0;
    virtual int count() = 0;
    //virtual void imageChanged() = 0;
};
Q_DECLARE_INTERFACE (IDataContainer,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // IDATACONTAINER_H
