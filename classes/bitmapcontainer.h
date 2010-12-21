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
public:
    QImage *image(const QString &key);
    void setImage(const QString &key, QImage *image);
    void transform(const QString &key, int code);
    int count();
    QStringList keys() const;
signals:
    void imageChanged(const QString &key);
public slots:

};
//-----------------------------------------------------------------------------

#endif // BITMAPCONTAINER_H
