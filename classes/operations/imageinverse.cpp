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

#include "imageinverse.h"
#include <QImage>
#include "idocument.h"
#include "datacontainer.h"

namespace Operations {

ImageInverse::ImageInverse(QObject *parent)
    : QObject(parent)
{
}

bool ImageInverse::prepare(const IDocument *doc, const QStringList &keys)
{
    Q_UNUSED(doc)
    Q_UNUSED(keys)

    return true;
}

void ImageInverse::applyDocument(IDocument *doc)
{
    Q_UNUSED(doc)
}

void ImageInverse::applyItem(IDocument *doc, const QString &itemKey)
{
    const QImage *original = doc->dataContainer()->image(itemKey);
    QImage result(*original);
    result.invertPixels();
    doc->dataContainer()->setImage(itemKey, &result);
}

}
