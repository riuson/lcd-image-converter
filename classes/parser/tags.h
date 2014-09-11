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
template <class T1, class T2> class QMap;
template <class T1, class T2> class QHash;
//-----------------------------------------------------------------------------
class Tags
{
public:
    Tags();
    ~Tags();

    enum TagsEnum
    {
        Unknown = 0,

        DocumentDataType,
        DocumentName,
        DocumentNameWithoutSpaces,
        DocumentFilename,

        PrepareConversionType,
        PrepareMonoType,
        PrepareMonoEdge,
        PrepareScanMain,
        PrepareScanSub,
        PrepareInverse,
        PrepareUseBands,
        PrepareBandWidth,

        ImageSplitToRows,
        ImageBlockSize,
        ImageRleCompression,
        ImageByteOrder,

        FontUseBom,
        FontEncoding,

        FontFamily,
        FontSize,
        FontStyle,
        FontString,
        FontWidthType,
        FontAntiAliasing,

        TemplateFilename,

        OutputPresetName,
        OutputBlocksCount,
        OutputBitsPerPixel,
        OutputCharacterCode,
        OutputCharacterText,
        OutputImageData,
        OutputImagesCount,
        OutputComma,
        OutputDataIndent,
        OutputDataEOL,
        OutputImageWidth,
        OutputImageHeight,
        OutputImagesMaxWidth,
        OutputImagesMaxHeight,
        OutputImagesMaxBlocksCount,

        BlocksHeaderStart,
        BlocksHeaderEnd,
        BlocksImagesTableStart,
        BlocksImagesTableEnd,
        BlocksFontDefinitionStart,
        BlocksFontDefinitionEnd
    };

    TagsEnum parseTag(const QString &key) const;

    const QString tagValue(TagsEnum key) const;
    void setTagValue(TagsEnum key, const QString &value);
    void importValues(const Tags *other);

    bool find(const QString &text, int startIndex, int *resultIndex, int *nextIndex, TagsEnum *key, QString *content);

private:
    QHash <QString, TagsEnum> *mTagNameMap;
    QMap <TagsEnum, QString> *mTagValues;

    void initTagsMap();
};
//-----------------------------------------------------------------------------
#endif // TAGS_H
