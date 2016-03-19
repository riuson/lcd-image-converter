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

#include "parsedimagedata.h"

#include <QImage>
#include <QSet>

#include "tags.h"
#include "converterhelper.h"
#include "preset.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
ParsedImageData::ParsedImageData(Preset *preset, const QImage *image, const Tags &tags, QObject *parent) :
    QObject(parent)
{
    this->mTags = new Tags();

    // width and height must be written before image changes
    this->mTags->setTagValue(Tags::OutputImageWidth, QString("%1").arg(image->width()));
    this->mTags->setTagValue(Tags::OutputImageHeight, QString("%1").arg(image->height()));
    // parameters from external code
    this->mTags->setTagValue(Tags::OutputDataEOL, tags.tagValue(Tags::OutputDataEOL));
    this->mTags->setTagValue(Tags::OutputDataIndent, tags.tagValue(Tags::OutputDataIndent));

    QImage imagePrepared;
    ConverterHelper::prepareImage(preset, image, &imagePrepared);

    // conversion from image to strings
    QVector<quint32> sourceData;
    int sourceWidth, sourceHeight;
    ConverterHelper::pixelsData(preset, &imagePrepared, &sourceData, &sourceWidth, &sourceHeight);

    if (sourceData.size() > 0)
    {
        ConverterHelper::processPixels(preset, &sourceData);

        QVector<quint32> packedData;
        int packedWidth, packedHeight;
        ConverterHelper::packData(
                    preset,
                    &sourceData, sourceWidth, sourceHeight,
                    &packedData, &packedWidth, &packedHeight);

        QVector<quint32> reorderedData;
        int reorderedWidth, reorderedHeight;
        ConverterHelper::reorder(
                    preset,
                    &packedData, packedWidth, packedHeight,
                    &reorderedData, &reorderedWidth, &reorderedHeight);

        QVector<quint32> compressedData;
        int compressedWidth, compressedHeight;
        ConverterHelper::compressData(preset, &reorderedData, reorderedWidth, reorderedHeight, &compressedData, &compressedWidth, &compressedHeight);

        this->mTags->setTagValue(Tags::OutputBlocksCount, QString("%1").arg(compressedData.size()));

        QString dataString = ConverterHelper::dataToString(
                    preset,
                    &compressedData, compressedWidth, compressedHeight);
        dataString.replace("\n", this->mTags->tagValue(Tags::OutputDataEOL) + this->mTags->tagValue(Tags::OutputDataIndent));

        // end of conversion

        this->mTags->setTagValue(Tags::OutputImageData, dataString);

        // get hash
        QString hashStr = QString("data: %1, width: %2, height: %3").arg(dataString).arg(image->width()).arg(image->height());
        this->mHash = qHash(hashStr);
    }
    else
    {
        this->mTags->setTagValue(Tags::OutputImageWidth, QString("0"));
        this->mTags->setTagValue(Tags::OutputImageHeight, QString("0"));

        this->mTags->setTagValue(Tags::OutputBlocksCount, QString("0"));
        this->mTags->setTagValue(Tags::OutputImageData, QString());

        // get hash
        QString hashStr = QString("empty, width: %1, height: %2").arg(image->width()).arg(image->height());
        this->mHash = qHash(hashStr);
    }
}
//-----------------------------------------------------------------------------
ParsedImageData::~ParsedImageData()
{
    delete this->mTags;
}
//-----------------------------------------------------------------------------
const Tags *ParsedImageData::tags() const
{
    return this->mTags;
}
//-----------------------------------------------------------------------------
uint ParsedImageData::hash() const
{
    return this->mHash;
}
//-----------------------------------------------------------------------------
