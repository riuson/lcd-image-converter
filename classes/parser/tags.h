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

#ifndef TAGS_H
#define TAGS_H
//-----------------------------------------------------------------------------
#include <QString>
//-----------------------------------------------------------------------------
//template <class T1, class T2> class QMultiMap;
template <class T1, class T2> class QHash;
//-----------------------------------------------------------------------------
class Tags
{
public:
    Tags();
    ~Tags();

    enum TagsEnum
    {
        UNKNOWN = 0,

        DOCUMENT_DATA_TYPE,
        DOCUMENT_NAME,
        DOCUMENT_NAME_WITHOUT_SPACES,
        DOCUMENT_FILENAME,

        PREPARE_CONV_TYPE,
        PREPARE_MONO_TYPE,
        PREPARE_MONO_EDGE,
        PREPARE_SCAN_MAIN,
        PREPARE_SCAN_SUB,
        PREPARE_INVERSE,
        PREPARE_USE_BANDS,
        PREPARE_BAND_WIDTH,

        IMAGE_SPLIT_TO_ROWS,
        IMAGE_BLOCK_SIZE,
        IMAGE_RLE_COMPRESSION,
        IMAGE_BYTE_ORDER,

        FONT_STRING,
        FONT_USE_BOM,
        FONT_ENCODING,
        FONT_WIDTH_TYPE,

        TEMPLATES_FONT_FILE,
        TEMPLATES_IMAGE_FILE,

        OUTPUT_PRESET_NAME,
        OUTPUT_BLOCKS_COUNT,
        OUTPUT_BITS_PER_PIXEL,
        OUTPUT_CHARACTER_CODE,
        OUTPUT_CHARACTER_TEXT,
        OUTPUT_IMAGE_DATA,
        OUTPUT_IMAGES_COUNT,
        OUTPUT_COMMA,

        BLOCK_START_HEADER,
        BLOCK_END_HEADER,
        BLOCK_START_IMAGES_TABLE,
        BLOCK_END_IMAGES_TABLE,
        BLOCK_START_FONT_DEFINITION,
        BLOCK_END_FONT_DEFINITION
    };

    TagsEnum parseTag(const QString &key);

private:
    QHash <QString, TagsEnum> *mTagNameMap;
};
//-----------------------------------------------------------------------------
#endif // TAGS_H
