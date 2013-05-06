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
//-----------------------------------------------------------------------------
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "idocument.h"
#include "datacontainer.h"
#include "bitmaphelper.h"
#include "converterhelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "fontoptions.h"
#include "templateoptions.h"
#include "tags.h"
//-----------------------------------------------------------------------------
Parser::Parser(QObject *parent, TemplateType templateType) :
        QObject(parent)
{
    this->mPreset = new Preset(this);

    this->mSelectedPresetName = Preset::currentName();

    this->mPreset->load(this->mSelectedPresetName);

    if (templateType == TypeImage)
        this->mTemplateFileName = this->mPreset->templates()->image();
    else
        this->mTemplateFileName = this->mPreset->templates()->font();
}
//-----------------------------------------------------------------------------
Parser::~Parser()
{
    delete this->mPreset;
}
//-----------------------------------------------------------------------------
QString Parser::name()
{
    return this->mSelectedPresetName;
}
//-----------------------------------------------------------------------------
QString Parser::convert(IDocument *document, Tags &tags) const
{
    QString result;

    QString templateString;

    QFile file(this->mTemplateFileName);

    if (!file.exists())
        file.setFileName(":/templates/image_convert");

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        templateString = stream.readAll();
        file.close();
    }
    tags.setTagValue(Tags::TemplateFilename, file.fileName());

    QString prefix, suffix;
    this->imageParticles(templateString, &prefix, &suffix);
    tags.setTagValue(Tags::OutputDataIndent, prefix);
    tags.setTagValue(Tags::OutputDataEOL, suffix);

    this->addMatrixInfo(tags);

    this->addImagesInfo(tags, document);

    result = this->parse(templateString, tags, document);

    return result;
}
//-----------------------------------------------------------------------------
QString Parser::parse(const QString &templateString,
                      Tags &tags,
                      IDocument *doc) const
{
    QString result;

    Tags::TagsEnum tagKey = Tags::Unknown;
    int lastIndex = 0, foundIndex = 0, nextIndex = 0;
    QString content;
    while (tags.find(templateString, lastIndex, &foundIndex, &nextIndex, &tagKey, &content))
    {
        if (foundIndex > lastIndex)
        {
            result.append(templateString.mid(lastIndex, foundIndex - lastIndex));
        }

        switch (tagKey)
        {
        case Tags::BlocksHeaderStart:
        case Tags::BlocksFontDefinitionStart:
        {
            QString temp = this->parse(content, tags, doc);
            result.append(temp);
            break;
        }
        case Tags::BlocksImagesTableStart:
        {
            QString temp = this->parseImagesTable(content, tags, doc);
            result.append(temp);
            break;
        }
        default:
        {
            result.append(tags.tagValue(tagKey));
            break;
        }
        }
        lastIndex = nextIndex;
    }

    if (lastIndex < templateString.length() - 1)
    {
        result.append(templateString.right(templateString.length() - lastIndex));
    }

    return result;
}
//-----------------------------------------------------------------------------
QString Parser::parseImagesTable(const QString &templateString,
                                 Tags &tags,
                                 IDocument *doc) const
{
    QString result;

    DataContainer *data = doc->dataContainer();
    bool useBom = this->mPreset->font()->bom();
    QString encoding = this->mPreset->font()->encoding();
    CharactersSortOrder order = this->mPreset->font()->sortOrder();

    QString imageString;

    QStringList keys = data->keys();
    QStringList sortedKeys = this->sortKeysWithEncoding(keys, encoding, useBom, order);

    QListIterator<QString> it(sortedKeys);
    it.toFront();

    while (it.hasNext())
    {
        QString key = it.next();
        QImage image = QImage(*data->image(key));

        QString dataString = this->parseImage(&image, tags);


        QString charCode = this->hexCode(key, encoding, useBom);

        tags.setTagValue(Tags::OutputImageData, dataString);
        tags.setTagValue(Tags::OutputCharacterCode, charCode);
        if (it.hasNext())
            tags.setTagValue(Tags::OutputComma, ",");
        else
            tags.setTagValue(Tags::OutputComma, "");

        if (key.contains("@"))
        {
            key.replace("@", "(a)");
            tags.setTagValue(Tags::OutputCharacterText, key);
        }
        else
            tags.setTagValue(Tags::OutputCharacterText, key.left(1));

        imageString = this->parse(templateString, tags, doc);
        result.append(imageString);
    }

    return result;
}
//-----------------------------------------------------------------------------
QString Parser::parseImage(const QImage *image, Tags &tags) const
{
    // width and height must be written before image changes
    tags.setTagValue(Tags::OutputImageWidth, QString("%1").arg(image->width()));
    tags.setTagValue(Tags::OutputImageHeight, QString("%1").arg(image->height()));

    QImage imagePrepared;
    ConverterHelper::prepareImage(this->mPreset, image, &imagePrepared);

    // conversion from image to strings
    QVector<quint32> sourceData;
    int sourceWidth, sourceHeight;
    ConverterHelper::pixelsData(this->mPreset, &imagePrepared, &sourceData, &sourceWidth, &sourceHeight);

    ConverterHelper::processPixels(this->mPreset, &sourceData);

    QVector<quint32> packedData;
    int packedWidth, packedHeight;
    ConverterHelper::packData(
                this->mPreset,
                &sourceData, sourceWidth, sourceHeight,
                &packedData, &packedWidth, &packedHeight);

    QVector<quint32> reorderedData;
    int reorderedWidth, reorderedHeight;
    ConverterHelper::reorder(
                this->mPreset,
                &packedData, packedWidth, packedHeight,
                &reorderedData, &reorderedWidth, &reorderedHeight);

    QVector<quint32> compressedData;
    int compressedWidth, compressedHeight;
    ConverterHelper::compressData(this->mPreset, &reorderedData, reorderedWidth, reorderedHeight, &compressedData, &compressedWidth, &compressedHeight);

    tags.setTagValue(Tags::OutputBlocksCount, QString("%1").arg(compressedData.size()));

    QString dataString = ConverterHelper::dataToString(this->mPreset, &compressedData, compressedWidth, compressedHeight, "0x");
    dataString.replace("\n", tags.tagValue(Tags::OutputDataEOL) + tags.tagValue(Tags::OutputDataIndent));

    // end of conversion

    return dataString;
}
//-----------------------------------------------------------------------------
QString Parser::hexCode(const QString &key, const QString &encoding, bool bom) const
{
    QString result;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toAscii());

    QChar ch = key.at(0);
    QByteArray codeArray = codec->fromUnicode(&ch, 1);

    quint64 code = 0;
    for (int i = 0; i < codeArray.count() && i < 8; i++)
    {
        code = code << 8;
        code |= (quint8)codeArray.at(i);
    }

    if (encoding.contains("UTF-16"))
    {
        // reorder bytes
        quint64 a =
                ((code & 0x000000000000ff00ULL) >> 8) |
                ((code & 0x00000000000000ffULL) << 8);
        code &= 0xffffffffffff0000ULL;
        code |= a;

        if (bom)
        {
            // 0xfeff00c1
            result = QString("%1").arg(code, 8, 16, QChar('0'));
        }
        else
        {
            // 0x00c1
            code &= 0x000000000000ffffULL;
            result = QString("%1").arg(code, 4, 16, QChar('0'));
        }
    }
    else if (encoding.contains("UTF-32"))
    {
        // reorder bytes
        quint64 a =
                ((code & 0x00000000ff000000ULL) >> 24) |
                ((code & 0x0000000000ff0000ULL) >> 8) |
                ((code & 0x000000000000ff00ULL) << 8) |
                ((code & 0x00000000000000ffULL) << 24);
        code &= 0xffffffff00000000ULL;
        code |= a;

        if (bom)
        {
            // 0x0000feff000000c1
            result = QString("%1").arg(code, 16, 16, QChar('0'));
        }
        else
        {
            // 0x000000c1
            code &= 0x00000000ffffffffULL;
            result = QString("%1").arg(code, 8, 16, QChar('0'));
        }
    }
    else
    {
        result = QString("%1").arg(code, codeArray.count() * 2, 16, QChar('0'));
    }


    return result;
}
//-----------------------------------------------------------------------------
void Parser::addMatrixInfo(Tags &tags) const
{
    // byte order
    if (this->mPreset->image()->bytesOrder() == BytesOrderLittleEndian)
        tags.setTagValue(Tags::ImageByteOrder, "little-endian");
    else
        tags.setTagValue(Tags::ImageByteOrder, "big-endian");

    // data block size
    int dataBlockSize = (this->mPreset->image()->blockSize() + 1) * 8;
    tags.setTagValue(Tags::ImageBlockSize, QString("%1").arg(dataBlockSize));

    // scan main direction
    switch (this->mPreset->prepare()->scanMain())
    {
        case TopToBottom:
            tags.setTagValue(Tags::PrepareScanMain, "top_to_bottom");
            break;
        case BottomToTop:
            tags.setTagValue(Tags::PrepareScanMain, "bottom_to_top");
            break;
        case LeftToRight:
            tags.setTagValue(Tags::PrepareScanMain, "left_to_right");
            break;
        case RightToLeft:
            tags.setTagValue(Tags::PrepareScanMain, "right_to_left");
            break;
    }

    // scan sub direction
    if (this->mPreset->prepare()->scanSub())
        tags.setTagValue(Tags::PrepareScanSub, "forward");
    else
        tags.setTagValue(Tags::PrepareScanSub, "backward");

    // bands
    if (this->mPreset->prepare()->bandScanning())
        tags.setTagValue(Tags::PrepareUseBands, "yes");
    else
        tags.setTagValue(Tags::PrepareUseBands, "no");
    int bandWidth = this->mPreset->prepare()->bandWidth();
    tags.setTagValue(Tags::PrepareBandWidth, QString("%1").arg(bandWidth));


    // inversion
    if (this->mPreset->prepare()->inverse())
        tags.setTagValue(Tags::PrepareInverse, "yes");
    else
        tags.setTagValue(Tags::PrepareInverse, "no");

    // bom
    if (this->mPreset->font()->bom())
        tags.setTagValue(Tags::FontUseBom, "yes");
    else
        tags.setTagValue(Tags::FontUseBom, "no");

    // encoding
    tags.setTagValue(Tags::FontEncoding, this->mPreset->font()->encoding());

    // split to rows
    if (this->mPreset->image()->splitToRows())
        tags.setTagValue(Tags::ImageSplitToRows, "yes");
    else
        tags.setTagValue(Tags::ImageSplitToRows, "no");

    // compression
    if (this->mPreset->image()->compressionRle())
        tags.setTagValue(Tags::ImageRleCompression, "yes");
    else
        tags.setTagValue(Tags::ImageRleCompression, "no");

    // preset name
    tags.setTagValue(Tags::OutputPresetName, this->mSelectedPresetName);

    // conversion type
    tags.setTagValue(Tags::PrepareConversionType, this->mPreset->prepare()->convTypeName());

    // monochrome type
    if (this->mPreset->prepare()->convType() == ConversionTypeMonochrome)
    {
        tags.setTagValue(Tags::PrepareMonoType, this->mPreset->prepare()->monoTypeName());
        tags.setTagValue(Tags::PrepareMonoEdge, QString("%1").arg(this->mPreset->prepare()->edge()));
    }
    else
    {
        tags.setTagValue(Tags::PrepareMonoType, "not_used");
        tags.setTagValue(Tags::PrepareMonoEdge, "not_used");
    }

    // bits per pixel
    quint32 maskUsed = this->mPreset->matrix()->maskUsed();
    int bitsPerPixel = 0;
    while (maskUsed > 0)
    {
        if ((maskUsed & 0x00000001) != 0)
            bitsPerPixel++;
        maskUsed = maskUsed >> 1;
    }
    tags.setTagValue(Tags::OutputBitsPerPixel, QString("%1").arg(bitsPerPixel));
}
//-----------------------------------------------------------------------------
void Parser::addImagesInfo(Tags &tags, IDocument *doc) const
{
    DataContainer *data = doc->dataContainer();
    QListIterator<QString> it(data->keys());
    it.toFront();

    int maxWidth = 0, maxHeight = 0;

    while (it.hasNext())
    {
        const QString key = it.next();
        QImage image = QImage(*data->image(key));

        if (image.width() > maxWidth)
        {
            maxWidth = image.width();
        }
        if (image.height() > maxHeight)
        {
            maxHeight = image.height();
        }
    }
    tags.setTagValue(Tags::OutputImagesCount, QString("%1").arg(data->count()));
    tags.setTagValue(Tags::OutputImagesMaxWidth, QString("%1").arg(maxWidth));
    tags.setTagValue(Tags::OutputImagesMaxHeight, QString("%1").arg(maxHeight));
}
//-----------------------------------------------------------------------------
void Parser::imageParticles(const QString &templateString, QString *prefix, QString *suffix) const
{
    QString templateOutImageData;

    // extract 'out_image_data' line
    QRegExp regOutImageData = QRegExp("[^\\n\\r]*out_image_data[^\\n\\r]*[\\n\\r]*");
    if (regOutImageData.indexIn(templateString) >= 0)
    {
        templateOutImageData = regOutImageData.cap();
    }
    else
    {
        regOutImageData.setPattern("[^\\n\\r]*imageData[^\\n\\r]*[\\n\\r]*");
        if (regOutImageData.indexIn(templateString) >= 0)
        {
            templateOutImageData = regOutImageData.cap();
        }
    }

    *prefix = QString("    ");
    *suffix = QString("\r\n");

    if (!templateOutImageData.isEmpty())
    {
        QRegExp regIndent = QRegExp("^[\\t\\ ]*");
        if (regIndent.indexIn(templateOutImageData) >= 0)
        {
            QString result = regIndent.cap();
            if (!result.isEmpty())
            {
                *prefix = result;
            }
        }

        QRegExp regEOL = QRegExp("[\\r\\n]*$");
        if (regEOL.indexIn(templateOutImageData) >= 0)
        {
            QString result = regEOL.cap();
            if (!result.isEmpty())
            {
                *suffix = result;
            }
        }
    }
}
//-----------------------------------------------------------------------------
bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}
//-----------------------------------------------------------------------------
bool caseInsensitiveMoreThan(const QString &s1, const QString &s2)
{
    return s1.toLower() > s2.toLower();
}
//-----------------------------------------------------------------------------
const QStringList Parser::sortKeysWithEncoding(
        const QStringList &keys,
        const QString &encoding,
        bool useBom,
        CharactersSortOrder order) const
{
    if (order == CharactersSortNone)
        return keys;

    QMap <QString, QString> map;

    QListIterator<QString> it(keys);
    it.toFront();

    while (it.hasNext())
    {
        const QString key = it.next();
        const QString hex = this->hexCode(key, encoding, useBom);
        map.insert(hex, key);
    }

    QStringList hexCodes = map.keys();

    switch (order)
    {
    case CharactersSortAscending:
        qSort(hexCodes.begin(), hexCodes.end(), caseInsensitiveLessThan);
        break;
    case CharactersSortDescending:
        qSort(hexCodes.begin(), hexCodes.end(), caseInsensitiveMoreThan);
        break;
    default:
        break;
    }

    QStringList result;
    it = QListIterator<QString>(hexCodes);
    it.toFront();

    while (it.hasNext())
    {
        const QString key = it.next();
        result.append(map.value(key));
    }

    return result;
}
//-----------------------------------------------------------------------------
