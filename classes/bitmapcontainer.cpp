#include "bitmapcontainer.h"

#include <QColor>
#include <QImage>

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
BitmapContainer::BitmapContainer(QObject *parent) :
    QObject(parent)
{
    //this->mImage = NULL;
    this->mImage = new QImage(":/images/template");
    this->mImage = new QImage(this->mImage->scaled(20, 10));
    //this->createNew(10, 10);
}
//-----------------------------------------------------------------------------
BitmapContainer::~BitmapContainer()
{
    if (this->mImage != NULL)
        delete this->mImage;
}
//-----------------------------------------------------------------------------
void BitmapContainer::createNew(int width, int height)
{
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(width, height, QImage::Format_RGB32);
}
//-----------------------------------------------------------------------------
QImage *BitmapContainer::image(int index)
{
    Q_UNUSED(index);
    return this->mImage;
}
//-----------------------------------------------------------------------------
void BitmapContainer::setImage(int index, QImage *image)
{
    Q_UNUSED(index);
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(*image);

    emit this->imageChanged(0);
}
//-----------------------------------------------------------------------------
void BitmapContainer::transform(int index, int code)
{
    Q_UNUSED(index);
    BitmapHelper::BitmapHelperTransformCodes type = (BitmapHelper::BitmapHelperTransformCodes)code;
    QImage result = BitmapHelper::transform(type, this->mImage);
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(result);

    emit this->imageChanged(0);
}
//-----------------------------------------------------------------------------
int BitmapContainer::count()
{
    return 1;
}
//-----------------------------------------------------------------------------
