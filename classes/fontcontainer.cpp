#include "fontcontainer.h"

#include <QImage>

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
FontContainer::FontContainer(QObject *parent) :
    QObject(parent)
{
    this->mDefaultImage = new QImage(":/images/template");
    //this->mImageMap.insert("default", this->mDefaultImage);
}
//-----------------------------------------------------------------------------
FontContainer::~FontContainer()
{
    qDeleteAll(this->mImageMap);
}
//-----------------------------------------------------------------------------
QImage *FontContainer::image(const QString &key)
{
    return this->mImageMap.value(key, this->mDefaultImage);
}
//-----------------------------------------------------------------------------
void FontContainer::setImage(const QString &key, QImage *image)
{
    this->remove(key);
    QImage *imageNew = new QImage(*image);
    this->mImageMap.insert(key, imageNew);
    emit this->imageChanged(key);
}
//-----------------------------------------------------------------------------
void FontContainer::transform(const QString &key, int code)
{
    QImage *imageOld = this->mImageMap.value(key);

    BitmapHelper::BitmapHelperTransformCodes type = (BitmapHelper::BitmapHelperTransformCodes)code;
    QImage result = BitmapHelper::transform(type, imageOld);

    this->mImageMap.remove(key);
    delete imageOld;

    QImage *imageNew = new QImage(result);
    this->mImageMap.insert(key, imageNew);

    emit this->imageChanged(0);
}
//-----------------------------------------------------------------------------
int FontContainer::count()
{
    return this->mImageMap.count();
}
//-----------------------------------------------------------------------------
void FontContainer::clear()
{
    qDeleteAll(this->mImageMap);
    this->mImageMap.clear();
}
//-----------------------------------------------------------------------------
QStringList FontContainer::keys() const
{
    QList<QString> tmp = this->mImageMap.keys();
    qSort(tmp);
    QStringList result(tmp);
    return result;
}
//-----------------------------------------------------------------------------
void FontContainer::remove(const QString &key)
{
    if (this->mImageMap.contains(key))
    {
        QImage *imageOld = this->mImageMap.value(key);
        this->mImageMap.remove(key);
        delete imageOld;
    }
}
//-----------------------------------------------------------------------------
