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

#include "tagslist.h"

#include <QMap>
#include <QHash>
#include <QRegExp>

namespace Parsing
{

TagsList::TagsList()
{
  this->initTagsMap();
  this->mTagValues = new QMap<Tag, QString>();
}

TagsList::~TagsList()
{
  this->mTagNameMap->clear();
  this->mTagNameMap->clear();

  delete this->mTagNameMap;
  delete this->mTagValues;
}

TagsList::Tag TagsList::parseTag(const QString &key) const
{
  return this->mTagNameMap->value(key, Tag::Unknown);
}

const QString TagsList::tagValue(TagsList::Tag key) const
{
  //return this->mTagValues->value(key, QString("<value not defined>"));
  QString res = this->mTagValues->value(key, QString("<value not defined>"));

  if (res.contains("defined")) {

  }

  return res;
}

void TagsList::setTagValue(TagsList::Tag key, const QString &value)
{
  this->mTagValues->insert(key, value);
}

void TagsList::importValues(const TagsList *other)
{
  QListIterator<Tag> it(other->mTagValues->keys());
  it.toFront();

  while (it.hasNext()) {
    Tag tag = it.next();
    this->setTagValue(tag, other->tagValue(tag));
  }
}

bool TagsList::find(const QString &text, int startIndex, int *resultIndex, int *nextIndex, Tag *key, QString *content)
{
  *resultIndex = -1;
  *nextIndex = -1;
  *key = Tag::Unknown;
  *content = QString();

  QRegExp reg("(\\@|\\$\\()(.+)(\\@|\\))");
  reg.setMinimal(true);

  int pos = reg.indexIn(text, startIndex);

  if (pos >= 0) {
    QString tagText = reg.cap(2);
    *key = this->parseTag(tagText);
    *resultIndex = pos;

    if (*key == Tag::BlocksHeaderStart ||
        *key == Tag::BlocksImagesTableStart ||
        *key == Tag::BlocksFontDefinitionStart) {
      QString blockName = tagText.remove(0, 12);
      QRegExp regContent = QRegExp("(\\@|\\$\\()start_block_" + blockName + "(\\@|\\))(.+)(\\@|\\$\\()end_block_" + blockName + "(\\@|\\))");
      regContent.setMinimal(true);

      if (regContent.indexIn(text, pos) >= 0) {
        QString contentText = regContent.cap(3);
        *content = contentText;//.trimmed();
        *nextIndex = pos + regContent.cap().length();
      }
    } else {
      *content = tagText;
      *nextIndex = pos + reg.cap().length();
    }

    return true;
  }

  return false;
}

void TagsList::initTagsMap()
{
  this->mTagNameMap = new QHash<QString, Tag>();

  this->mTagNameMap->insert("doc_data_type",   Tag::DocumentDataType);
  this->mTagNameMap->insert("dataType",        Tag::DocumentDataType);
  this->mTagNameMap->insert("doc_name",        Tag::DocumentName);
  this->mTagNameMap->insert("documentName",    Tag::DocumentName);
  this->mTagNameMap->insert("doc_name_ws",     Tag::DocumentNameWithoutSpaces);
  this->mTagNameMap->insert("documentName_ws", Tag::DocumentNameWithoutSpaces);
  this->mTagNameMap->insert("doc_filename",    Tag::DocumentFilename);
  this->mTagNameMap->insert("fileName",        Tag::DocumentFilename);

  this->mTagNameMap->insert("pre_conv_type",  Tag::PrepareConversionType);
  this->mTagNameMap->insert("convType",       Tag::PrepareConversionType);
  this->mTagNameMap->insert("pre_mono_type",  Tag::PrepareMonoType);
  this->mTagNameMap->insert("monoType",       Tag::PrepareMonoType);
  this->mTagNameMap->insert("pre_mono_edge",  Tag::PrepareMonoEdge);
  this->mTagNameMap->insert("edge",           Tag::PrepareMonoEdge);
  this->mTagNameMap->insert("pre_scan_main",  Tag::PrepareScanMain);
  this->mTagNameMap->insert("scanMain",       Tag::PrepareScanMain);
  this->mTagNameMap->insert("pre_scan_sub",   Tag::PrepareScanSub);
  this->mTagNameMap->insert("scanSub",        Tag::PrepareScanSub);
  this->mTagNameMap->insert("pre_inverse",    Tag::PrepareInverse);
  this->mTagNameMap->insert("inverse",        Tag::PrepareInverse);
  this->mTagNameMap->insert("pre_use_bands",  Tag::PrepareUseBands);
  this->mTagNameMap->insert("bands",          Tag::PrepareUseBands);
  this->mTagNameMap->insert("pre_band_width", Tag::PrepareBandWidth);
  this->mTagNameMap->insert("bandWidth",      Tag::PrepareBandWidth);

  this->mTagNameMap->insert("img_split_to_rows",   Tag::ImageSplitToRows);
  this->mTagNameMap->insert("splitToRows",         Tag::ImageSplitToRows);
  this->mTagNameMap->insert("img_data_block_size", Tag::ImageBlockSize);
  this->mTagNameMap->insert("dataBlockSize",       Tag::ImageBlockSize);
  this->mTagNameMap->insert("img_rle",             Tag::ImageRleCompression);
  this->mTagNameMap->insert("rle",                 Tag::ImageRleCompression);
  this->mTagNameMap->insert("img_byte_order",      Tag::ImageByteOrder);
  this->mTagNameMap->insert("bytesOrder",          Tag::ImageByteOrder);

  this->mTagNameMap->insert("fnt_use_bom",    Tag::FontUseBom);
  this->mTagNameMap->insert("bom",            Tag::FontUseBom);
  this->mTagNameMap->insert("fnt_encoding",   Tag::FontEncoding);
  this->mTagNameMap->insert("encoding",       Tag::FontEncoding);

  this->mTagNameMap->insert("fnt_family",       Tag::FontFamily);
  this->mTagNameMap->insert("fontFamily",       Tag::FontFamily);
  this->mTagNameMap->insert("fnt_size",         Tag::FontSize);
  this->mTagNameMap->insert("fontSize",         Tag::FontSize);
  this->mTagNameMap->insert("fnt_style",        Tag::FontStyle);
  this->mTagNameMap->insert("fontStyle",        Tag::FontStyle);
  this->mTagNameMap->insert("fnt_string",       Tag::FontString);
  this->mTagNameMap->insert("string",           Tag::FontString);
  this->mTagNameMap->insert("fnt_width_type",   Tag::FontWidthType);
  this->mTagNameMap->insert("fontWidthType",    Tag::FontWidthType);
  this->mTagNameMap->insert("fnt_antialiasing", Tag::FontAntiAliasing);
  this->mTagNameMap->insert("fontAntialiasing", Tag::FontAntiAliasing);
  this->mTagNameMap->insert("fnt_ascent",       Tag::FontAscent);
  this->mTagNameMap->insert("fnt_descent",      Tag::FontDescent);

  this->mTagNameMap->insert("tmpl_filename", Tag::TemplateFilename);
  this->mTagNameMap->insert("templateFile",  Tag::TemplateFilename);

  this->mTagNameMap->insert("out_preset_name",       Tag::OutputPresetName);
  this->mTagNameMap->insert("preset",                Tag::OutputPresetName);
  this->mTagNameMap->insert("out_blocks_count",      Tag::OutputBlocksCount);
  this->mTagNameMap->insert("blocksCount",           Tag::OutputBlocksCount);
  this->mTagNameMap->insert("out_bpp",               Tag::OutputBitsPerPixel);
  this->mTagNameMap->insert("bpp",                   Tag::OutputBitsPerPixel);
  this->mTagNameMap->insert("out_char_code",         Tag::OutputCharacterCode);
  this->mTagNameMap->insert("charCode",              Tag::OutputCharacterCode);
  this->mTagNameMap->insert("out_char_code_sim",     Tag::OutputCharacterCodeSimilar);
  this->mTagNameMap->insert("out_char_text",         Tag::OutputCharacterText);
  this->mTagNameMap->insert("charText",              Tag::OutputCharacterText);
  this->mTagNameMap->insert("out_char_text_sim",     Tag::OutputCharacterTextSimilar);
  this->mTagNameMap->insert("out_char_index",        Tag::OutputCharacterIndex);
  this->mTagNameMap->insert("out_char_offset",       Tag::OutputCharacterOffset);
  this->mTagNameMap->insert("out_char_glyph_x",      Tag::OutputCharacterGlyphX);
  this->mTagNameMap->insert("out_char_glyph_y",      Tag::OutputCharacterGlyphY);
  this->mTagNameMap->insert("out_char_glyph_width",  Tag::OutputCharacterGlyphWidth);
  this->mTagNameMap->insert("out_char_glyph_height", Tag::OutputCharacterGlyphHeight);
  this->mTagNameMap->insert("out_image_data",        Tag::OutputImageData);
  this->mTagNameMap->insert("imageData",             Tag::OutputImageData);
  this->mTagNameMap->insert("out_image_preview",     Tag::OutputImagePreview);
  this->mTagNameMap->insert("out_images_count",      Tag::OutputImagesCount);
  this->mTagNameMap->insert("imagesCount",           Tag::OutputImagesCount);
  this->mTagNameMap->insert("out_comma",             Tag::OutputComma);
  this->mTagNameMap->insert("comma",                 Tag::OutputComma);
  this->mTagNameMap->insert("out_data_indent",       Tag::OutputDataIndent);
  this->mTagNameMap->insert("imageDataIndent",       Tag::OutputDataIndent);
  this->mTagNameMap->insert("out_image_width",       Tag::OutputImageWidth);
  this->mTagNameMap->insert("width",                 Tag::OutputImageWidth);
  this->mTagNameMap->insert("out_image_height",      Tag::OutputImageHeight);
  this->mTagNameMap->insert("height",                Tag::OutputImageHeight);
  this->mTagNameMap->insert("out_images_max_width",  Tag::OutputImagesMaxWidth);
  this->mTagNameMap->insert("out_images_max_height", Tag::OutputImagesMaxHeight);
  this->mTagNameMap->insert("out_images_max_blocks_count", Tag::OutputImagesMaxBlocksCount);

  this->mTagNameMap->insert("app_date", Tag::ApplicationDate);
  this->mTagNameMap->insert("app_rev", Tag::ApplicationRevision);

  this->mTagNameMap->insert("start_block_header",       Tag::BlocksHeaderStart);
  this->mTagNameMap->insert("end_block_header",         Tag::BlocksHeaderEnd);
  this->mTagNameMap->insert("start_block_images_table", Tag::BlocksImagesTableStart);
  this->mTagNameMap->insert("end_block_images_table",   Tag::BlocksImagesTableEnd);
  this->mTagNameMap->insert("start_block_font_def",     Tag::BlocksFontDefinitionStart);
  this->mTagNameMap->insert("end_block_font_def",       Tag::BlocksFontDefinitionEnd);
}

} // namespace Parsing
