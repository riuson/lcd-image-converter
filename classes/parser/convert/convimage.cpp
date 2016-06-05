/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "convimage.h"
//-----------------------------------------------------------------------------
#include <QImage>
//-----------------------------------------------------------------------------
ConvImage::ConvImage(const QImage *image, QObject *parent) :
    QObject(parent)
{
    this->mImage = image;
    this->mBandSize = 1;
    this->mUseBands = false;
}
//-----------------------------------------------------------------------------
ConvImage::~ConvImage()
{
}
//-----------------------------------------------------------------------------
int ConvImage::bandSize() const
{
    return this->mBandSize;
}
//-----------------------------------------------------------------------------
void ConvImage::setBandSize(int value)
{
    this->mBandSize = value;
}
//-----------------------------------------------------------------------------
bool ConvImage::useBands() const
{
    return this->mUseBands;
}
//-----------------------------------------------------------------------------
void ConvImage::setUseBands(bool value)
{
    this->mUseBands= value;
}
//-----------------------------------------------------------------------------
void ConvImage::addPoint(int x, int y)
{
    this->mPoints.append(QPoint(x, y));
}
//-----------------------------------------------------------------------------
void ConvImage::clearPoints()
{
    this->mPoints.clear();
}
//-----------------------------------------------------------------------------
QPoint ConvImage::pointAt(int index) const
{
    if (index < this->mPoints.size())
    {
        return this->mPoints.at(index);
    }
    return QPoint();
}
//-----------------------------------------------------------------------------
int ConvImage::pointsCount() const
{
    return this->mPoints.size();
}
//-----------------------------------------------------------------------------
bool ConvImage::needBreakScan() const
{
    // 120% of points or 100k
    int maxCount = qMax((this->mImage->width() * this->mImage->height() * 120) / 100, 100000);
    return this->pointsCount() > maxCount;
}
//-----------------------------------------------------------------------------
int ConvImage::height() const
{
    return this->mImage->height();
}
//-----------------------------------------------------------------------------
int ConvImage::width() const
{
    return this->mImage->width();
}
//-----------------------------------------------------------------------------
