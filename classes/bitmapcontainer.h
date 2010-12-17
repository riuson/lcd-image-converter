#ifndef BITMAPCONTAINER_H
#define BITMAPCONTAINER_H

#include <QObject>
//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------

class BitmapContainer : public QObject
{
Q_OBJECT
public:
    explicit BitmapContainer(QObject *parent = 0);
private:
    QImage *mImage;
    void createNew(quint32 width, quint32 height);
public:
    void inverse();
    QImage *image();
signals:

public slots:

};
//-----------------------------------------------------------------------------

#endif // BITMAPCONTAINER_H
