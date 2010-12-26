/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

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
