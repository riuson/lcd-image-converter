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

#include "imagegrayscale.h"
#include <QImage>
#include "idocument.h"
#include "datacontainer.h"
#include "converterhelper.h"

namespace Operations
{

ImageGrayscale::ImageGrayscale(QObject *parent) : QObject(parent)
{
}

bool ImageGrayscale::prepare(const IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)

  return true;
}

void ImageGrayscale::applyDocument(IDocument *doc, const QStringList &keys)
{
  Q_UNUSED(doc)
  Q_UNUSED(keys)
}

void ImageGrayscale::applyItem(IDocument *doc, const QString &itemKey)
{
  const QImage *original = doc->dataContainer()->image(itemKey);
  QImage result(*original);
  ConverterHelper::makeGrayscale(result);
  doc->dataContainer()->setImage(itemKey, &result);
}

}
