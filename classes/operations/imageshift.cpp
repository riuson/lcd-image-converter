/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "imageshift.h"
#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace Operations
{

ImageShift::ImageShift(QObject *parent) : QObject(parent)
{
    this->mDirection = Direction::None;
}

bool ImageShift::prepare(const IDocument *doc, const QStringList &keys)
{
    Q_UNUSED(doc)
    Q_UNUSED(keys)
    return true;
}

void ImageShift::applyDocument(IDocument *doc)
{
    Q_UNUSED(doc)
}

void ImageShift::applyItem(IDocument *doc, const QString &itemKey)
{
    const QImage *original = doc->dataContainer()->image(itemKey);
    QImage result;

    switch (this->mDirection)
    {
    case Direction::Left:
        result = BitmapHelper::shiftLeft(original);
        break;
    case Direction::Right:
        result = BitmapHelper::shiftRight(original);
        break;
    case Direction::Up:
        result = BitmapHelper::shiftUp(original);
        break;
    case Direction::Down:
        result = BitmapHelper::shiftDown(original);
        break;
    case Direction::None:
    default:
        result = *original;
        break;
    }

    doc->dataContainer()->setImage(itemKey, &result);
}

void ImageShift::setDirection(ImageShift::Direction direction)
{
    this->mDirection = direction;
}

}
