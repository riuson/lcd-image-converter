#ifndef FONTCONTAINER_H
#define FONTCONTAINER_H

#include <QObject>

#include <QList>

#include "idatacontainer.h"
//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------
class FontContainer : public QObject, public IDataContainer
{
    Q_OBJECT
    Q_INTERFACES(IDataContainer)
public:
    explicit FontContainer(QObject *parent = 0);
    ~FontContainer();
private:
    QList<QImage *> mImageList;
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
#endif // FONTCONTAINER_H
