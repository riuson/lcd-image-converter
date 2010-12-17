#include "bitmapcontainer.h"

#include <QColor>
#include <QImage>
//-----------------------------------------------------------------------------
BitmapContainer::BitmapContainer(QObject *parent) :
    QObject(parent)
{
    //this->mImage = NULL;
    this->mImage = new QImage(":/images/template");
    //this->createNew(10, 10);
}
//-----------------------------------------------------------------------------
void BitmapContainer::createNew(quint32 width, quint32 height)
{
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(width, height, QImage::Format_RGB32);
}
//-----------------------------------------------------------------------------
void BitmapContainer::inverse()
{
    this->mImage->invertPixels();
}
//-----------------------------------------------------------------------------
