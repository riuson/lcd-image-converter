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

#include "imagefliphorizontal.h"
#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace Operations
{

ImageFlipHorizontal::ImageFlipHorizontal(QObject *parent)
    : QObject(parent)
{
}

bool ImageFlipHorizontal::prepare(const IDocument *doc)
{
    Q_UNUSED(doc)
    return true;
}

void ImageFlipHorizontal::applyDocument(IDocument *doc)
{
    Q_UNUSED(doc)
}

void ImageFlipHorizontal::applyItem(IDocument *doc, const QString &itemKey)
{
    const QImage *original = doc->dataContainer()->image(itemKey);
    QImage result = BitmapHelper::flipHorizontal(original);
    doc->dataContainer()->setImage(itemKey, &result);
}

}
