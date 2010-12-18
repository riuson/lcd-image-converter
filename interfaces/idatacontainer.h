#ifndef IDATACONTAINER_H
#define IDATACONTAINER_H
//-----------------------------------------------------------------------------
#include <QtGlobal>
class QImage;
//-----------------------------------------------------------------------------
class IDataContainer
{
public:
    virtual QImage *image(int index) = 0;
    virtual void setImage(int index, QImage *image) = 0;
    virtual void transform(int index, int code) = 0;
    virtual int count() = 0;
    //virtual void imageChanged() = 0;
};
Q_DECLARE_INTERFACE (IDataContainer,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // IDATACONTAINER_H
