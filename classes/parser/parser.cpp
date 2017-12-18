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

#include "parser.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"
#include "fonthelper.h"
#include "converterhelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "fontoptions.h"
#include "templateoptions.h"
#include "tags.h"
#include "parsedimagedata.h"
#include "revisioninfo.h"

/*
 -- Algorithm --

 Action handler :: convert()
 Select filename to save
 Load preset
 IDocument::convert()
 Save result string to file

 Document :: convert()
 Collect tags from document properties
 Sort document keys by specified encoduing, bom and order
 Prepare images data (ParsedImageData*) and save to list
 Create parser instance
 Pass document, sorted keys, image data and tags to parser::convert()
 Return result string

 Document :: prepareImages()
 Collect ParsedImageDate list
 Add tags of every particular image to its ParsedImageData
 Find duplicates by image's hash and store in image's tags
 Return list

 Parser::convert()
 Read template from file to string
 Get data indent and eol from template
 Parser::prepareImages()
 Parser::addMatrixInfo()
 Parser::addImagesInfo()
 Result := Parser::parse()
 return result string;

 Parser::parse()
 Loop get next tag and inner content
   If tag found
   Append substring from previous tag to result
   If tag type == (Font Definion Start || Header Start)
     Pass content to Parser::parse()
   EndIf
   If tag type == Images Table Start
     Pass content to Parser::parseImagesTable()
   EndIf
   If tag type == Simple
     Get value of the tag
   EndIf
   Append string to result
 EndLoop
 Append substring from last tag to end to result
 Return updated tags list

 Parser::parseImagesTable()
 Sort images by encoding, bom and order
 Loop
   If can get next image key
   Get ParsedImageData by key
   Add tags from ParsedImageData to tags
   Pass template substring from ImagesTable to Parser::parse()
   Append parsed string to result
 EndLoop
 Return list of ParsedImageData
 */

namespace Parsing
{

Parser::Parser(TemplateType templateType, Preset *preset, QObject *parent) :
  QObject(parent)
{
  this->mPreset = preset;

  //this->mSelectedPresetName = Preset::currentName();

  //this->mPreset->load(this->mSelectedPresetName);

  if (templateType == TypeImage) {
    this->mTemplateFileName = this->mPreset->templates()->image();
  } else {
    this->mTemplateFileName = this->mPreset->templates()->font();
  }
}

Parser::~Parser()
{
}

QString Parser::convert(Data::Containers::IDocument *document, const QStringList &orderedKeys, QMap<QString, ParsedImageData *> *images, Tags &tags) const
{
  QString result;

  QString templateString;

  QFile file(this->mTemplateFileName);

  if (!file.exists()) {
    file.setFileName(":/templates/image_convert");
  }

  if (file.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file);
    templateString = stream.readAll();
    file.close();
  }

  tags.setTagValue(Tags::TemplateFilename, file.fileName());

  QString prefix, suffix;
  this->imageDataParticles(templateString, &prefix, &suffix);
  tags.setTagValue(Tags::OutputDataIndent, prefix);
  tags.setTagValue(Tags::OutputDataEOL, suffix);

  this->imagePreviewParticles(templateString, &prefix, &suffix);
  tags.setTagValue(Tags::OutputPreviewIndent, prefix);
  tags.setTagValue(Tags::OutputPreviewEOL, suffix);

  this->addCommonInfo(tags);
  this->addMatrixInfo(tags);
  this->addImagesInfo(tags, images);

  result = this->parse(templateString, tags, document, orderedKeys, images);

  return result;
}

QString Parser::parse(const QString &templateString,
                      Tags &tags,
                      Data::Containers::IDocument *doc,
                      const QStringList &orderedKeys,
                      QMap<QString, ParsedImageData *> *images) const
{
  QString result;

  Tags::TagsEnum tagKey = Tags::Unknown;
  int lastIndex = 0, foundIndex = 0, nextIndex = 0;
  QString content;

  while (tags.find(templateString, lastIndex, &foundIndex, &nextIndex, &tagKey, &content)) {
    if (foundIndex > lastIndex) {
      result.append(templateString.mid(lastIndex, foundIndex - lastIndex));
    }

    switch (tagKey) {
      case Tags::BlocksHeaderStart:
      case Tags::BlocksFontDefinitionStart: {
        QString temp = this->parse(content, tags, doc, orderedKeys, images);
        result.append(temp);
        break;
      }

      case Tags::BlocksImagesTableStart: {
        QString temp = this->parseImagesTable(content, tags, doc, orderedKeys, images);
        result.append(temp);
        break;
      }

      default: {
        result.append(tags.tagValue(tagKey));
        break;
      }
    }

    lastIndex = nextIndex;
  }

  if (lastIndex < templateString.length() - 1) {
    result.append(templateString.right(templateString.length() - lastIndex));
  }

  return result;
}

QString Parser::parseImagesTable(const QString &templateString,
                                 Tags &tags,
                                 Data::Containers::IDocument *doc,
                                 const QStringList &orderedKeys,
                                 QMap<QString, ParsedImageData *> *images) const
{
  QString result;

  QListIterator<QString> it(orderedKeys);
  it.toFront();

  while (it.hasNext()) {
    QString key = it.next();

    ParsedImageData *data = images->value(key);

    if (data != nullptr) {
      tags.importValues(data->tags());

      if (it.hasNext()) {
        tags.setTagValue(Tags::OutputComma, ",");
      } else {
        tags.setTagValue(Tags::OutputComma, "");
      }

      QString imageString = this->parse(templateString, tags, doc, orderedKeys, images);
      result.append(imageString);
    }
  }

  return result;
}

QString Parser::hexCode(const QString &key, const QString &encoding, bool bom) const
{
  QString result;
  QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());

  QChar ch = key.at(0);
  QByteArray codeArray = codec->fromUnicode(&ch, 1);

  quint64 code = 0;

  for (int i = 0; i < codeArray.count() && i < 8; i++) {
    code = code << 8;
    code |= (quint8)codeArray.at(i);
  }

  if (encoding.contains("UTF-16")) {
    // reorder bytes
    quint64 a =
      ((code & 0x000000000000ff00ULL) >> 8) |
      ((code & 0x00000000000000ffULL) << 8);
    code &= 0xffffffffffff0000ULL;
    code |= a;

    if (bom) {
      // 0xfeff00c1
      result = QString("%1").arg(code, 8, 16, QChar('0'));
    } else {
      // 0x00c1
      code &= 0x000000000000ffffULL;
      result = QString("%1").arg(code, 4, 16, QChar('0'));
    }
  } else if (encoding.contains("UTF-32")) {
    // reorder bytes
    quint64 a =
      ((code & 0x00000000ff000000ULL) >> 24) |
      ((code & 0x0000000000ff0000ULL) >> 8) |
      ((code & 0x000000000000ff00ULL) << 8) |
      ((code & 0x00000000000000ffULL) << 24);
    code &= 0xffffffff00000000ULL;
    code |= a;

    if (bom) {
      // 0x0000feff000000c1
      result = QString("%1").arg(code, 16, 16, QChar('0'));
    } else {
      // 0x000000c1
      code &= 0x00000000ffffffffULL;
      result = QString("%1").arg(code, 8, 16, QChar('0'));
    }
  } else {
    result = QString("%1").arg(code, codeArray.count() * 2, 16, QChar('0'));
  }

  return result;
}

void Parser::addMatrixInfo(Tags &tags) const
{
  // byte order
  if (this->mPreset->image()->bytesOrder() == Parsing::Conversion::Options::BytesOrder::LittleEndian) {
    tags.setTagValue(Tags::ImageByteOrder, "little-endian");
  } else {
    tags.setTagValue(Tags::ImageByteOrder, "big-endian");
  }

  // data block size
  int dataBlockSize = (static_cast<int>(this->mPreset->image()->blockSize()) + 1) * 8;
  tags.setTagValue(Tags::ImageBlockSize, QString("%1").arg(dataBlockSize));

  // scan main direction
  switch (this->mPreset->prepare()->scanMain()) {
    case Parsing::Conversion::Options::ScanMainDirection::TopToBottom:
      tags.setTagValue(Tags::PrepareScanMain, "top_to_bottom");
      break;

    case Parsing::Conversion::Options::ScanMainDirection::BottomToTop:
      tags.setTagValue(Tags::PrepareScanMain, "bottom_to_top");
      break;

    case Parsing::Conversion::Options::ScanMainDirection::LeftToRight:
      tags.setTagValue(Tags::PrepareScanMain, "left_to_right");
      break;

    case Parsing::Conversion::Options::ScanMainDirection::RightToLeft:
      tags.setTagValue(Tags::PrepareScanMain, "right_to_left");
      break;
  }

  // scan sub direction
  if (this->mPreset->prepare()->scanSub() == Parsing::Conversion::Options::ScanSubDirection::Forward) {
    tags.setTagValue(Tags::PrepareScanSub, "forward");
  } else {
    tags.setTagValue(Tags::PrepareScanSub, "backward");
  }

  // bands
  if (this->mPreset->prepare()->bandScanning()) {
    tags.setTagValue(Tags::PrepareUseBands, "yes");
  } else {
    tags.setTagValue(Tags::PrepareUseBands, "no");
  }

  int bandWidth = this->mPreset->prepare()->bandWidth();
  tags.setTagValue(Tags::PrepareBandWidth, QString("%1").arg(bandWidth));

  // inversion
  if (this->mPreset->prepare()->inverse()) {
    tags.setTagValue(Tags::PrepareInverse, "yes");
  } else {
    tags.setTagValue(Tags::PrepareInverse, "no");
  }

  // bom
  if (this->mPreset->font()->bom()) {
    tags.setTagValue(Tags::FontUseBom, "yes");
  } else {
    tags.setTagValue(Tags::FontUseBom, "no");
  }

  // encoding
  tags.setTagValue(Tags::FontEncoding, this->mPreset->font()->encoding());

  // split to rows
  if (this->mPreset->image()->splitToRows()) {
    tags.setTagValue(Tags::ImageSplitToRows, "yes");
  } else {
    tags.setTagValue(Tags::ImageSplitToRows, "no");
  }

  // compression
  if (this->mPreset->image()->compressionRle()) {
    tags.setTagValue(Tags::ImageRleCompression, "yes");
  } else {
    tags.setTagValue(Tags::ImageRleCompression, "no");
  }

  // preset name
  tags.setTagValue(Tags::OutputPresetName, this->mPreset->name());

  // conversion type
  tags.setTagValue(Tags::PrepareConversionType, this->mPreset->prepare()->convTypeName());

  // monochrome type
  if (this->mPreset->prepare()->convType() == Parsing::Conversion::Options::ConversionType::Monochrome) {
    tags.setTagValue(Tags::PrepareMonoType, this->mPreset->prepare()->monoTypeName());
    tags.setTagValue(Tags::PrepareMonoEdge, QString("%1").arg(this->mPreset->prepare()->edge()));
  } else {
    tags.setTagValue(Tags::PrepareMonoType, "not_used");
    tags.setTagValue(Tags::PrepareMonoEdge, "not_used");
  }

  // bits per pixel
  quint32 maskUsed = this->mPreset->matrix()->maskUsed();
  int bitsPerPixel = 0;

  while (maskUsed > 0) {
    if ((maskUsed & 0x00000001) != 0) {
      bitsPerPixel++;
    }

    maskUsed = maskUsed >> 1;
  }

  tags.setTagValue(Tags::OutputBitsPerPixel, QString("%1").arg(bitsPerPixel));
}

void Parser::addImagesInfo(Tags &tags, QMap<QString, ParsedImageData *> *images) const
{
  QListIterator<QString> it(images->keys());
  it.toFront();

  int maxWidth = 0, maxHeight = 0, maxBlocksCount = 0;

  while (it.hasNext()) {
    const QString key = it.next();
    ParsedImageData *data = images->value(key);

    if (data != nullptr) {
      bool ok;
      int width = data->tags()->tagValue(Tags::OutputImageWidth).toInt(&ok);

      if (ok) {
        int height = data->tags()->tagValue(Tags::OutputImageHeight).toInt(&ok);

        if (ok) {
          int blocksCount = data->tags()->tagValue(Tags::OutputBlocksCount).toInt(&ok);

          if (ok) {
            if (width > maxWidth) {
              maxWidth = width;
            }

            if (height > maxHeight) {
              maxHeight = height;
            }

            if (blocksCount > maxBlocksCount) {
              maxBlocksCount = blocksCount;
            }
          }
        }
      }

      // apply imageParticles to outputImageData
      data->tags()->setTagValue(Tags::OutputImageData, data->outputImageDataWithEOL(tags));
      data->tags()->setTagValue(Tags::OutputImagePreview, data->outputImagePreviewWithEOL(tags));
    }
  }

  tags.setTagValue(Tags::OutputImagesCount, QString("%1").arg(images->count()));
  tags.setTagValue(Tags::OutputImagesMaxWidth, QString("%1").arg(maxWidth));
  tags.setTagValue(Tags::OutputImagesMaxHeight, QString("%1").arg(maxHeight));
  tags.setTagValue(Tags::OutputImagesMaxBlocksCount, QString("%1").arg(maxBlocksCount));
}

void Parser::addCommonInfo(Tags &tags) const
{
  tags.setTagValue(Tags::ApplicationDate, QString("%1").arg(VersionControl::RevisionInfo::date()));
  tags.setTagValue(Tags::ApplicationRevision, QString("%1").arg(VersionControl::RevisionInfo::hash_abbr()));
}

void Parser::imageDataParticles(const QString &templateString, QString *prefix, QString *suffix) const
{
  QString templateOutImageData;

  // extract 'out_image_data' line
  QRegExp regOutImageData = QRegExp("[^\\n\\r]*out_image_data[^\\n\\r]*[\\n\\r]*");

  if (regOutImageData.indexIn(templateString) >= 0) {
    templateOutImageData = regOutImageData.cap();
  } else {
    regOutImageData.setPattern("[^\\n\\r]*imageData[^\\n\\r]*[\\n\\r]*");

    if (regOutImageData.indexIn(templateString) >= 0) {
      templateOutImageData = regOutImageData.cap();
    }
  }

  *prefix = QString("    ");
  *suffix = QString("\r\n");

  if (!templateOutImageData.isEmpty()) {
    QRegExp regIndent = QRegExp("^[\\t\\ ]*");

    if (regIndent.indexIn(templateOutImageData) >= 0) {
      QString result = regIndent.cap();

      if (!result.isEmpty()) {
        *prefix = result;
      }
    }

    QRegExp regEOL = QRegExp("[\\r\\n]*$");

    if (regEOL.indexIn(templateOutImageData) >= 0) {
      QString result = regEOL.cap();

      if (!result.isEmpty()) {
        *suffix = result;
      }
    }
  }
}

void Parser::imagePreviewParticles(const QString &templateString, QString *prefix, QString *suffix) const
{
  QString templateOutImagePreview;

  // extract 'out_image_data' line
  QRegExp regOutImageData = QRegExp("[^\\n\\r]*out_image_preview[^\\n\\r]*[\\n\\r]*");

  if (regOutImageData.indexIn(templateString) >= 0) {
    templateOutImagePreview = regOutImageData.cap();
  }

  *prefix = QString("    ");
  *suffix = QString("\r\n");

  if (!templateOutImagePreview.isEmpty()) {
    QRegExp regIndent = QRegExp("^[\\t\\ ]*");

    if (regIndent.indexIn(templateOutImagePreview) >= 0) {
      QString result = regIndent.cap();

      if (!result.isEmpty()) {
        *prefix = result;
      }
    }

    QRegExp regEOL = QRegExp("[\\r\\n]*$");

    if (regEOL.indexIn(templateOutImagePreview) >= 0) {
      QString result = regEOL.cap();

      if (!result.isEmpty()) {
        *suffix = result;
      }
    }
  }
}

} // namespace Parsing
