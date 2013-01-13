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
    return this->mTagNameMap->value(key, UNKNOWN);
}
//-----------------------------------------------------------------------------
const QString Tags::tagValue(Tags::TagsEnum key)
{
    return this->mTagValues->value(key, QString("none"));
}
//-----------------------------------------------------------------------------
void Tags::setTagValue(Tags::TagsEnum key, const QString &value)
{
    this->mTagValues->insert(key, value);
}
//-----------------------------------------------------------------------------
void Tags::initTagsMap()
{
    this->mTagNameMap = new QHash<QString, TagsEnum>();

    this->mTagNameMap->insert("doc_data_type",   DOCUMENT_DATA_TYPE);
    this->mTagNameMap->insert("dataType",        DOCUMENT_DATA_TYPE);
    this->mTagNameMap->insert("doc_name",        DOCUMENT_NAME);
    this->mTagNameMap->insert("documentName",    DOCUMENT_NAME);
    this->mTagNameMap->insert("doc_name_ws",     DOCUMENT_NAME_WITHOUT_SPACES);
    this->mTagNameMap->insert("documentName_ws", DOCUMENT_NAME_WITHOUT_SPACES);
    this->mTagNameMap->insert("doc_filename",    DOCUMENT_FILENAME);
    this->mTagNameMap->insert("fileName",        DOCUMENT_FILENAME);

    this->mTagNameMap->insert("pre_conv_type",  PREPARE_CONV_TYPE);
    this->mTagNameMap->insert("convType",       PREPARE_CONV_TYPE);
    this->mTagNameMap->insert("pre_mono_type",  PREPARE_MONO_TYPE);
    this->mTagNameMap->insert("monoType",       PREPARE_MONO_TYPE);
    this->mTagNameMap->insert("pre_mono_edge",  PREPARE_MONO_EDGE);
    this->mTagNameMap->insert("edge",           PREPARE_MONO_EDGE);
    this->mTagNameMap->insert("pre_scan_main",  PREPARE_SCAN_MAIN);
    this->mTagNameMap->insert("scanMain",       PREPARE_SCAN_MAIN);
    this->mTagNameMap->insert("pre_scan_sub",   PREPARE_SCAN_SUB);
    this->mTagNameMap->insert("scanSub",        PREPARE_SCAN_SUB);
    this->mTagNameMap->insert("pre_inverse",    PREPARE_INVERSE);
    this->mTagNameMap->insert("inverse",        PREPARE_INVERSE);
    this->mTagNameMap->insert("pre_use_bands",  PREPARE_USE_BANDS);
    this->mTagNameMap->insert("bands",          PREPARE_USE_BANDS);
    this->mTagNameMap->insert("pre_band_width", PREPARE_BAND_WIDTH);
    this->mTagNameMap->insert("bandWidth",      PREPARE_BAND_WIDTH);

    this->mTagNameMap->insert("img_split_to_rows",   IMAGE_SPLIT_TO_ROWS);
    this->mTagNameMap->insert("splitToRows",         IMAGE_SPLIT_TO_ROWS);
    this->mTagNameMap->insert("img_data_block_size", IMAGE_BLOCK_SIZE);
    this->mTagNameMap->insert("dataBlockSize",       IMAGE_BLOCK_SIZE);
    this->mTagNameMap->insert("img_rle_compression", IMAGE_RLE_COMPRESSION);
    this->mTagNameMap->insert("rle",                 IMAGE_RLE_COMPRESSION);
    this->mTagNameMap->insert("img_byte_order",      IMAGE_BYTE_ORDER);

    this->mTagNameMap->insert("fnt_string",     FONT_STRING);
    this->mTagNameMap->insert("string",         FONT_STRING);
    this->mTagNameMap->insert("fnt_use_bom",    FONT_USE_BOM);
    this->mTagNameMap->insert("bom",            FONT_USE_BOM);
    this->mTagNameMap->insert("fnt_encoding",   FONT_ENCODING);
    this->mTagNameMap->insert("encoding",       FONT_ENCODING);
    this->mTagNameMap->insert("fnt_width_type", FONT_WIDTH_TYPE);
    this->mTagNameMap->insert("fontWidthType",  FONT_WIDTH_TYPE);

    this->mTagNameMap->insert("tmpl_font_filename",  TEMPLATES_FONT_FILE);
    this->mTagNameMap->insert("tmpl_image_filename", TEMPLATES_IMAGE_FILE);

    this->mTagNameMap->insert("out_preset_name",  OUTPUT_PRESET_NAME);
    this->mTagNameMap->insert("preset",           OUTPUT_PRESET_NAME);
    this->mTagNameMap->insert("out_blocks_count", OUTPUT_BLOCKS_COUNT);
    this->mTagNameMap->insert("blocksCount",      OUTPUT_BLOCKS_COUNT);
    this->mTagNameMap->insert("out_bpp",          OUTPUT_BITS_PER_PIXEL);
    this->mTagNameMap->insert("bpp",              OUTPUT_BITS_PER_PIXEL);
    this->mTagNameMap->insert("out_char_code",    OUTPUT_CHARACTER_CODE);
    this->mTagNameMap->insert("charCode",         OUTPUT_CHARACTER_CODE);
    this->mTagNameMap->insert("out_char_text",    OUTPUT_CHARACTER_TEXT);
    this->mTagNameMap->insert("charText",         OUTPUT_CHARACTER_TEXT);
    this->mTagNameMap->insert("out_image_data",   OUTPUT_IMAGE_DATA);
    this->mTagNameMap->insert("imageData",        OUTPUT_IMAGE_DATA);
    this->mTagNameMap->insert("out_images_count", OUTPUT_IMAGES_COUNT);
    this->mTagNameMap->insert("imagesCount",      OUTPUT_IMAGES_COUNT);
    this->mTagNameMap->insert("out_comma",        OUTPUT_COMMA);
    this->mTagNameMap->insert("comma",            OUTPUT_COMMA);

    this->mTagNameMap->insert("start_block_header",       BLOCK_START_HEADER);
    this->mTagNameMap->insert("end_block_header",         BLOCK_END_HEADER);
    this->mTagNameMap->insert("start_block_images_table", BLOCK_START_IMAGES_TABLE);
    this->mTagNameMap->insert("end_block_images_table",   BLOCK_END_IMAGES_TABLE);
    this->mTagNameMap->insert("start_block_font_def",     BLOCK_START_FONT_DEFINITION);
    this->mTagNameMap->insert("end_block_font_def",       BLOCK_END_FONT_DEFINITION);
}
//-----------------------------------------------------------------------------
