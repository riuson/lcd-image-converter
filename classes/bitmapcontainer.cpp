/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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

#include "bitmapcontainer.h"

#include <QColor>
#include <QImage>

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
BitmapContainer::BitmapContainer(QObject *parent) :
    QObject(parent)
{
    this->mImage = new QImage(QImage(":/images/template").scaled(20, 10));
}
//-----------------------------------------------------------------------------
BitmapContainer::~BitmapContainer()
{
    if (this->mImage != NULL)
        delete this->mImage;
}
//-----------------------------------------------------------------------------
QImage *BitmapContainer::image(const QString &key) const
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
int BitmapContainer::count() const
{
    return 1;
}
//-----------------------------------------------------------------------------
QStringList BitmapContainer::keys() const
{
    static const QStringList result("default");
    return result;
}
//-----------------------------------------------------------------------------
