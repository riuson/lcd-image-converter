#ifndef BITMAPCONTAINER_H
#define BITMAPCONTAINER_H

#include <QObject>

#include "idatacontainer.h"
//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------
class BitmapContainer : public QObject, public IDataContainer
{
    Q_OBJECT
    Q_INTERFACES(IDataContainer)
public:
    explicit BitmapContainer(QObject *parent = 0);
    ~BitmapContainer();
private:
    QImage *mImage;
    void createNew(int width, int height);
public:
    QImage *image(int index);
    void setImage(int index, QImage *image);
    void transform(int index, int code);
    int count();
signals:
    void imageChanged(int index);
public slots:

};
//-----------------------------------------------------------------------------

#endif // BITMAPCONTAINER_H
