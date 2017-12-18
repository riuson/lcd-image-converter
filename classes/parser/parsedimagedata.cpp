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

#include <QFile>
#include <QImage>
#include <QSet>
#include <QTextStream>

#include "tagslist.h"
#include "converterhelper.h"
#include "preset.h"
#include "imageoptions.h"

namespace Parsing
{

ParsedImageData::ParsedImageData(Preset *preset, const QImage *image, const TagsList &tags, QObject *parent) :
  QObject(parent)
{
  this->mTags = new TagsList();

  // width and height must be written before image changes
  this->mTags->setTagValue(TagsList::Tag::OutputImageWidth, QString("%1").arg(image->width()));
  this->mTags->setTagValue(TagsList::Tag::OutputImageHeight, QString("%1").arg(image->height()));
  // parameters from external code
  this->mTags->setTagValue(TagsList::Tag::OutputDataEOL, tags.tagValue(TagsList::Tag::OutputDataEOL));
  this->mTags->setTagValue(TagsList::Tag::OutputDataIndent, tags.tagValue(TagsList::Tag::OutputDataIndent));

  QImage imagePrepared;
  Parsing::Conversion::ConverterHelper::prepareImage(preset, image, &imagePrepared);

  // conversion from image to strings
  QVector<quint32> sourceData;
  int sourceWidth, sourceHeight;
  Parsing::Conversion::ConverterHelper::pixelsData(
    preset->prepare(),
    Parsing::Conversion::ConverterHelper::scanScript(preset),
    Parsing::Conversion::ConverterHelper::pixelsScript(preset),
    &imagePrepared,
    &sourceData,
    &sourceWidth,
    &sourceHeight);

  if (sourceData.size() > 0) {
    Parsing::Conversion::ConverterHelper::processPixels(preset, &sourceData);

    QVector<quint32> packedData;
    int packedWidth, packedHeight;
    Parsing::Conversion::ConverterHelper::packData(
      preset,
      &sourceData, sourceWidth, sourceHeight,
      &packedData, &packedWidth, &packedHeight);

    QVector<quint32> reorderedData;
    int reorderedWidth, reorderedHeight;
    Parsing::Conversion::ConverterHelper::reorder(
      preset,
      &packedData, packedWidth, packedHeight,
      &reorderedData, &reorderedWidth, &reorderedHeight);

    QVector<quint32> compressedData;
    int compressedWidth, compressedHeight;
    Parsing::Conversion::ConverterHelper::compressData(
      preset,
      &reorderedData,
      reorderedWidth,
      reorderedHeight,
      &compressedData,
      &compressedWidth,
      &compressedHeight);

    this->mTags->setTagValue(TagsList::Tag::OutputBlocksCount, QString("%1").arg(compressedData.size()));

    this->mPreparedOutputImageData = Parsing::Conversion::ConverterHelper::dataToString(
                                       preset,
                                       &compressedData, compressedWidth, compressedHeight);

    // get hash
    QString hashStr = QString("data: %1, width: %2, height: %3").arg(this->mPreparedOutputImageData).arg(image->width()).arg(image->height());
    this->mHash = qHash(hashStr);

    // Preview
    // Scan script
    QFile filePreviewScanScript(":/scan_scripts/t2b_f");
    QString previewScanScript;

    if (filePreviewScanScript.open(QIODevice::ReadOnly)) {
      QTextStream stream(&filePreviewScanScript);
      previewScanScript = stream.readAll();
      filePreviewScanScript.close();
    }

    // Pixels script
    QFile filePreviewPixelsScript(":/scan_scripts/pixels_example");
    QString previewPixelsScript;

    if (filePreviewPixelsScript.open(QIODevice::ReadOnly)) {
      QTextStream stream(&filePreviewPixelsScript);
      previewPixelsScript = stream.readAll();
      filePreviewPixelsScript.close();
    }

    // Collect pixels to make simple preview
    QVector<quint32> previewData;
    int previewWidth, previewHeight;
    Parsing::Conversion::ConverterHelper::pixelsData(preset->prepare(), previewScanScript, previewPixelsScript, &imagePrepared, &previewData, &previewWidth, &previewHeight);

    this->mPreparedOutputImagePreview = Parsing::Conversion::ConverterHelper::previewDataToString(
                                          preset,
                                          &previewData, previewWidth, previewHeight);
  } else {
    this->mTags->setTagValue(TagsList::Tag::OutputImageWidth, QString("0"));
    this->mTags->setTagValue(TagsList::Tag::OutputImageHeight, QString("0"));

    this->mTags->setTagValue(TagsList::Tag::OutputBlocksCount, QString("0"));
    this->mPreparedOutputImageData = QString();

    // get hash
    QString hashStr = QString("empty, width: %1, height: %2").arg(image->width()).arg(image->height());
    this->mHash = qHash(hashStr);
  }
}

ParsedImageData::~ParsedImageData()
{
  delete this->mTags;
}

TagsList *ParsedImageData::tags() const
{
  return this->mTags;
}

uint ParsedImageData::hash() const
{
  return this->mHash;
}

const QString ParsedImageData::outputImageDataWithEOL(const TagsList &tags) const
{
  QString result = this->mPreparedOutputImageData;
  result.replace("\n", tags.tagValue(TagsList::Tag::OutputDataEOL) + tags.tagValue(TagsList::Tag::OutputDataIndent));
  return result;
}

const QString ParsedImageData::outputImagePreviewWithEOL(const TagsList &tags) const
{
  QString result = this->mPreparedOutputImagePreview;
  result.replace("\n", tags.tagValue(TagsList::Tag::OutputPreviewEOL) + tags.tagValue(TagsList::Tag::OutputPreviewIndent));
  return result;
}

} // namespace Parsing
