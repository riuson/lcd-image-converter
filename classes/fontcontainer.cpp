#include "fontcontainer.h"

#include <QImage>

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
FontContainer::FontContainer(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
FontContainer::~FontContainer()
{
    qDeleteAll(this->mImageList);
}
//-----------------------------------------------------------------------------
QImage *FontContainer::image(int index)
{
    return this->mImageList.at(index);
}
//-----------------------------------------------------------------------------
void FontContainer::setImage(int index, QImage *image)
{
    QImage *imageOld = this->mImageList.at(index);
    this->mImageList.removeAt(index);
    delete imageOld;
    this->mImageList.insert(index, image);
    emit this->imageChanged(index);
}
//-----------------------------------------------------------------------------
void FontContainer::transform(int index, int code)
{
    QImage *imageOld = this->mImageList.at(index);

    BitmapHelper::BitmapHelperTransformCodes type = (BitmapHelper::BitmapHelperTransformCodes)code;
    QImage result = BitmapHelper::transform(type, imageOld);

    this->mImageList.removeAt(index);
    delete imageOld;

    QImage *imageNew = new QImage(result);
    this->mImageList.insert(index, imageNew);

    emit this->imageChanged(0);
}
//-----------------------------------------------------------------------------
int FontContainer::count()
{
    return this->mImageList.count();
}
//-----------------------------------------------------------------------------
