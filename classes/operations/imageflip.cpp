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

#include "imageflip.h"
#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace Operations
{

ImageFlip::ImageFlip(QObject *parent)
  : QObject(parent)
{
  this->mFlipHorizontal = false;
  this->mFlipVertical = false;
}

bool ImageFlip::prepare(const IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
  return true;
}

void ImageFlip::applyDocument(IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageFlip::applyItem(IDocument *doc, const QString &itemKey)
{
  const QImage *original = doc->dataContainer()->image(itemKey);
  QImage result = *original;

  if (this->mFlipHorizontal) {
    result = BitmapHelper::flipHorizontal(&result);
  }

  if (this->mFlipVertical) {
    result = BitmapHelper::flipVertical(&result);
  }

  doc->dataContainer()->setImage(itemKey, &result);
}

void ImageFlip::setOrientation(bool flipHorizontal, bool flipVertical)
{
  this->mFlipHorizontal = flipHorizontal;
  this->mFlipVertical = flipVertical;
}

}
