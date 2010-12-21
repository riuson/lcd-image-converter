#include "bitmapcontainer.h"

#include <QColor>
#include <QImage>

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
BitmapContainer::BitmapContainer(QObject *parent) :
    QObject(parent)
{
    //this->mImage = NULL;
    //this->mImage = new QImage(":/images/template");
    this->mImage = new QImage(QImage(":/images/template").scaled(20, 10));
    //this->createNew(10, 10);
}
//-----------------------------------------------------------------------------
BitmapContainer::~BitmapContainer()
{
    if (this->mImage != NULL)
        delete this->mImage;
}
//-----------------------------------------------------------------------------
QImage *BitmapContainer::image(const QString &key)
{
    Q_UNUSED(key);
    return this->mImage;
}
//-----------------------------------------------------------------------------
void BitmapContainer::setImage(const QString &key, QImage *image)
{
    Q_UNUSED(key);
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(*image);

    emit this->imageChanged("default");
}
//-----------------------------------------------------------------------------
void BitmapContainer::transform(const QString &key, int code)
{
    Q_UNUSED(key);
    BitmapHelper::BitmapHelperTransformCodes type = (BitmapHelper::BitmapHelperTransformCodes)code;
    QImage result = BitmapHelper::transform(type, this->mImage);
    if (this->mImage != NULL)
        delete this->mImage;
    this->mImage = new QImage(result);

    emit this->imageChanged("default");
}
//-----------------------------------------------------------------------------
int BitmapContainer::count()
{
    return 1;
}
//-----------------------------------------------------------------------------
QStringList BitmapContainer::keys() const
{
    QStringList result("default");
    return result;
}
//-----------------------------------------------------------------------------
