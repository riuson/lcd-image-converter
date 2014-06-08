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

#include "tags.h"

#include <QMap>
#include <QHash>
#include <QRegExp>
//-----------------------------------------------------------------------------
Tags::Tags()
{
    this->initTagsMap();
    this->mTagValues = new QMap<TagsEnum, QString>();
}
//-----------------------------------------------------------------------------
Tags::~Tags()
{
    this->mTagNameMap->clear();
}
//-----------------------------------------------------------------------------
Tags::TagsEnum Tags::parseTag(const QString &key) const
{
    return this->mTagNameMap->value(key, Unknown);
}
//-----------------------------------------------------------------------------
const QString Tags::tagValue(Tags::TagsEnum key) const
{
    return this->mTagValues->value(key, QString("<value not defined>"));
}
//-----------------------------------------------------------------------------
void Tags::setTagValue(Tags::TagsEnum key, const QString &value)
{
    this->mTagValues->insert(key, value);
}
//-----------------------------------------------------------------------------
void Tags::importValues(const Tags *other)
{
    QListIterator<TagsEnum> it(other->mTagValues->keys());
    it.toFront();

    while (it.hasNext())
    {
        TagsEnum tag = it.next();
        this->setTagValue(tag, other->tagValue(tag));
    }
}
//-----------------------------------------------------------------------------
bool Tags::find(const QString &text, int startIndex, int *resultIndex, int *nextIndex, TagsEnum *key, QString *content)
{
    *resultIndex = -1;
    *nextIndex = -1;
    *key = Unknown;
    *content = QString();

    QRegExp reg("(\\@|\\$\\()(.+)(\\@|\\))");
    reg.setMinimal(true);

    int pos = reg.indexIn(text, startIndex);
    if (pos >= 0)
    {
        QString tagText = reg.cap(2);
        *key = this->parseTag(tagText);
        *resultIndex = pos;

        if (*key == BlocksHeaderStart ||
            *key == BlocksImagesTableStart ||
            *key == BlocksFontDefinitionStart)
        {
            QString blockName = tagText.remove(0, 12);
            QRegExp regContent = QRegExp("(\\@|\\$\\()start_block_" + blockName + "(\\@|\\))(.+)(\\@|\\$\\()end_block_" + blockName + "(\\@|\\))");
            regContent.setMinimal(true);
            if (regContent.indexIn(text, pos) >= 0)
            {
                QString contentText = regContent.cap(3);
                *content = contentText;//.trimmed();
                *nextIndex = pos + regContent.cap().length();
            }
        }
        else
        {
            *content = tagText;
            *nextIndex = pos + reg.cap().length();
        }

        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
void Tags::initTagsMap()
{
    this->mTagNameMap = new QHash<QString, TagsEnum>();

    this->mTagNameMap->insert("doc_data_type",   DocumentDataType);
    this->mTagNameMap->insert("dataType",        DocumentDataType);
    this->mTagNameMap->insert("doc_name",        DocumentName);
    this->mTagNameMap->insert("documentName",    DocumentName);
    this->mTagNameMap->insert("doc_name_ws",     DocumentNameWithoutSpaces);
    this->mTagNameMap->insert("documentName_ws", DocumentNameWithoutSpaces);
    this->mTagNameMap->insert("doc_filename",    DocumentFilename);
    this->mTagNameMap->insert("fileName",        DocumentFilename);

    this->mTagNameMap->insert("pre_conv_type",  PrepareConversionType);
    this->mTagNameMap->insert("convType",       PrepareConversionType);
    this->mTagNameMap->insert("pre_mono_type",  PrepareMonoType);
    this->mTagNameMap->insert("monoType",       PrepareMonoType);
    this->mTagNameMap->insert("pre_mono_edge",  PrepareMonoEdge);
    this->mTagNameMap->insert("edge",           PrepareMonoEdge);
    this->mTagNameMap->insert("pre_scan_main",  PrepareScanMain);
    this->mTagNameMap->insert("scanMain",       PrepareScanMain);
    this->mTagNameMap->insert("pre_scan_sub",   PrepareScanSub);
    this->mTagNameMap->insert("scanSub",        PrepareScanSub);
    this->mTagNameMap->insert("pre_inverse",    PrepareInverse);
    this->mTagNameMap->insert("inverse",        PrepareInverse);
    this->mTagNameMap->insert("pre_use_bands",  PrepareUseBands);
    this->mTagNameMap->insert("bands",          PrepareUseBands);
    this->mTagNameMap->insert("pre_band_width", PrepareBandWidth);
    this->mTagNameMap->insert("bandWidth",      PrepareBandWidth);

    this->mTagNameMap->insert("img_split_to_rows",   ImageSplitToRows);
    this->mTagNameMap->insert("splitToRows",         ImageSplitToRows);
    this->mTagNameMap->insert("img_data_block_size", ImageBlockSize);
    this->mTagNameMap->insert("dataBlockSize",       ImageBlockSize);
    this->mTagNameMap->insert("img_rle",             ImageRleCompression);
    this->mTagNameMap->insert("rle",                 ImageRleCompression);
    this->mTagNameMap->insert("img_byte_order",      ImageByteOrder);
    this->mTagNameMap->insert("bytesOrder",          ImageByteOrder);

    this->mTagNameMap->insert("fnt_use_bom",    FontUseBom);
    this->mTagNameMap->insert("bom",            FontUseBom);
    this->mTagNameMap->insert("fnt_encoding",   FontEncoding);
    this->mTagNameMap->insert("encoding",       FontEncoding);

    this->mTagNameMap->insert("fnt_family",       FontFamily);
    this->mTagNameMap->insert("fontFamily",       FontFamily);
    this->mTagNameMap->insert("fnt_size",         FontSize);
    this->mTagNameMap->insert("fontSize",         FontSize);
    //this->mTagNameMap->insert("fnt_style",        FontStyle); // replaced by fnt_bold & fnt_string
    //this->mTagNameMap->insert("fontStyle",        FontStyle);
    this->mTagNameMap->insert("fnt_bold",         FontBold);
    this->mTagNameMap->insert("fnt_italic",       FontItalic);
    this->mTagNameMap->insert("fnt_string",       FontString);
    this->mTagNameMap->insert("string",           FontString);
    this->mTagNameMap->insert("fnt_width_type",   FontWidthType);
    this->mTagNameMap->insert("fontWidthType",    FontWidthType);
    this->mTagNameMap->insert("fnt_antialiasing", FontAntiAliasing);
    this->mTagNameMap->insert("fontAntialiasing", FontAntiAliasing);

    this->mTagNameMap->insert("tmpl_filename", TemplateFilename);
    this->mTagNameMap->insert("templateFile",  TemplateFilename);

    this->mTagNameMap->insert("out_preset_name",  OutputPresetName);
    this->mTagNameMap->insert("preset",           OutputPresetName);
    this->mTagNameMap->insert("out_blocks_count", OutputBlocksCount);
    this->mTagNameMap->insert("blocksCount",      OutputBlocksCount);
    this->mTagNameMap->insert("out_bpp",          OutputBitsPerPixel);
    this->mTagNameMap->insert("bpp",              OutputBitsPerPixel);
    this->mTagNameMap->insert("out_char_code",    OutputCharacterCode);
    this->mTagNameMap->insert("charCode",         OutputCharacterCode);
    this->mTagNameMap->insert("out_char_text",    OutputCharacterText);
    this->mTagNameMap->insert("charText",         OutputCharacterText);
    this->mTagNameMap->insert("out_image_data",   OutputImageData);
    this->mTagNameMap->insert("imageData",        OutputImageData);
    this->mTagNameMap->insert("out_images_count", OutputImagesCount);
    this->mTagNameMap->insert("imagesCount",      OutputImagesCount);
    this->mTagNameMap->insert("out_comma",        OutputComma);
    this->mTagNameMap->insert("comma",            OutputComma);
    this->mTagNameMap->insert("out_data_indent",  OutputDataIndent);
    this->mTagNameMap->insert("imageDataIndent",  OutputDataIndent);
    this->mTagNameMap->insert("out_image_width",  OutputImageWidth);
    this->mTagNameMap->insert("width",            OutputImageWidth);
    this->mTagNameMap->insert("out_image_height", OutputImageHeight);
    this->mTagNameMap->insert("height",           OutputImageHeight);
    this->mTagNameMap->insert("out_images_max_width",  OutputImagesMaxWidth);
    this->mTagNameMap->insert("out_images_max_height", OutputImagesMaxHeight);
    this->mTagNameMap->insert("out_images_max_blocks_count", OutputImagesMaxBlocksCount);

    this->mTagNameMap->insert("start_block_header",       BlocksHeaderStart);
    this->mTagNameMap->insert("end_block_header",         BlocksHeaderEnd);
    this->mTagNameMap->insert("start_block_images_table", BlocksImagesTableStart);
    this->mTagNameMap->insert("end_block_images_table",   BlocksImagesTableEnd);
    this->mTagNameMap->insert("start_block_font_def",     BlocksFontDefinitionStart);
    this->mTagNameMap->insert("end_block_font_def",       BlocksFontDefinitionEnd);
}
//-----------------------------------------------------------------------------
