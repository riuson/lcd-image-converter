/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "converter.h"
//-----------------------------------------------------------------------------
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include "idocument.h"
#include "idatacontainer.h"
#include "bitmaphelper.h"
#include "conversionmatrix.h"
#include "conversionmatrixoptions.h"
#include "converterhelper.h"
//-----------------------------------------------------------------------------
Converter::Converter(QObject *parent) :
        QObject(parent)
{
    this->mMatrix = new ConversionMatrix(this);

    QSettings sett;
    sett.beginGroup("presets");
    this->mSelectedPresetName = sett.value("selected", QVariant("default")).toString();
    sett.endGroup();

    this->mMatrix->load(this->mSelectedPresetName);
}
//-----------------------------------------------------------------------------
Converter::~Converter()
{
    this->mConverters.clear();
}
//-----------------------------------------------------------------------------
QString Converter::name()
{
    //IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    //return options->name();
    return this->mSelectedPresetName;
}
//-----------------------------------------------------------------------------
QString Converter::convert(IDocument *document,
                           const QString &templateFile,
                           QMap<QString, QString> &tags)
{
    QString result;

    QString templateString;

    QFile file(templateFile);
    //if (!file.exists())
    //{
    //    file.setFileName(":/templates/image_convert");
    //}
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        templateString = stream.readAll();
        file.close();
    }
    tags.insert("templateFile", file.fileName());

    QRegExp regEnc("encoding:\\s+(.+)(?=\\s)");
    regEnc.setMinimal(true);
    if (regEnc.indexIn(templateString) >= 0)
    {
        QString enc = regEnc.cap(1);
        tags["encoding"] = enc.toUpper();
    }
    else
    {
        tags["encoding"] = "UTF-8";
    }

    QRegExp regBom("unicode bom:\\s+(.+)(?=\\s)");
    regBom.setMinimal(true);
    if (regBom.indexIn(templateString) >= 0)
    {
        QString bom = regBom.cap(1);
        tags["bom"] = bom.toUpper();
    }
    else
    {
        tags["bom"] = "NO";
    }

    this->addPreprocessInfo(tags);

    this->addOrderInfo(tags);

    this->parse(templateString, result, tags, document);

    return result;
}
//-----------------------------------------------------------------------------
void Converter::parse(const QString &templateString,
                      QString &resultString,
                      QMap<QString, QString> &tags,
                      IDocument *doc)
{
    int index = 0;
    int prevIndex = 0;
    QRegExp regTag("@(start_block_)?(.+)@");
    regTag.setMinimal(true);
    int capturedLength = 0;
    while ((index = regTag.indexIn(templateString, index + capturedLength)) >= 0)
    {
        capturedLength = regTag.cap(0).length();
        if (index > prevIndex)
        {
            resultString.append(templateString.mid(prevIndex, index - prevIndex));
        }
        QString tagName = regTag.cap(2);
        // if block starts
        if (regTag.cap(1) == "start_block_")
        {
            QRegExp contentReg("@start_block_" + tagName + "@(.+)@end_block_" + tagName + "@");
            contentReg.setMinimal(true);
            if (contentReg.indexIn(templateString, index) >= 0)
            {
                QString content = contentReg.cap(1);
                content = content.trimmed();
                QString temp;

                if (tagName == "images_table")
                {
                    this->parseImagesTable(content, temp, tags, doc);
                }
                else
                {
                    this->parse(content, temp, tags, doc);
                }
                resultString.append(temp);

                capturedLength = contentReg.cap(0).length();
                prevIndex = index + capturedLength;
            }
        }
        else
        {
            if (tags.contains(tagName))
                resultString.append(tags.value(tagName));
            else
                resultString.append("<value not defined>");
            prevIndex = index + regTag.cap(0).length();
        }
    }
    int last = prevIndex;
    if (last < templateString.length() - 1)
    {
        resultString.append(templateString.mid(last, templateString.length() - last));
    }
}
//-----------------------------------------------------------------------------
void Converter::parseBlocks(const QString &templateString,
                            QString &resultString,
                            QMap<QString, QString> &tags,
                            IDocument *doc)
{
    // capture block
    QRegExp startReg("@start_block_(.+)(?=\\s)");
    startReg.setMinimal(true);
    int index = -1;
    while ((index = startReg.indexIn(templateString, index + 1)) >= 0)
    {
        QString blockName = startReg.cap(1);
        QRegExp endReg("@end_block_" + blockName);
        endReg.setMinimal(true);
        // capture block's content
        QRegExp contentReg("@start_block_" + blockName + "(.+)@end_block_" + blockName);
        contentReg.setMinimal(true);
        int index2 = index - 1;
        while ((index2 = contentReg.indexIn(templateString, index2 + 1)) >= 0)
        {
            QString content = contentReg.cap(1);
            //index2 += content.length();
            content = content.trimmed();

            QString contentParsed;
            if (blockName == "images_table")
            {
                this->parseImagesTable(content, contentParsed, tags, doc);
            }
            else
            {
                this->parseSimple(content, contentParsed, tags, doc);
            }
            resultString.append(contentParsed);
        }
    }
}
//-----------------------------------------------------------------------------
void Converter::parseImagesTable(const QString &templateString,
                                 QString &resultString,
                                 QMap<QString, QString> &tags,
                                 IDocument *doc)
{
    IDataContainer *data = doc->dataContainer();
    QString imageString;
    QListIterator<QString> it(data->keys());
    it.toFront();
    tags["imagesCount"] = QString("%1").arg(data->count());
    while (it.hasNext())
    {
        QString key = it.next();
        QImage image = QImage(*data->image(key));

        // width and height must be written before image changes
        tags["width"] = QString("%1").arg(image.width());
        tags["height"] = QString("%1").arg(image.height());


        // conversion from image to strings
        QVector<quint32> imageData;
        int width, height;
        ConverterHelper::pixelsData(this->mMatrix, &image, &imageData, &width, &height);

        ConverterHelper::processPixels(this->mMatrix, &imageData);

        QVector<quint32> imageDataPacked;
        int width2, height2;
        ConverterHelper::packData(this->mMatrix, &imageData, width, height, &imageDataPacked, &width2, &height2);

        QString dataString = ConverterHelper::dataToString(this->mMatrix, &imageDataPacked, width2, height2);

        // end of conversion

        bool useBom = false;
        if (tags.value("bom") == "YES")
            useBom = true;

        QString charCode = this->hexCode(key.at(0), tags.value("encoding"), useBom);

        tags["blocksCount"] = QString("%1").arg(imageDataPacked.size());
        tags["imageData"] = dataString;
        tags["charCode"] = charCode;
        if (it.hasNext())
            tags["comma"] = ",";
        else
            tags["comma"] = "";

        if (key.contains("@"))
        {
            key.replace("@", "(a)");
            tags["charText"] = key;
        }
        else
            tags["charText"] = key.left(1);

        this->parseSimple(templateString, imageString, tags, doc);
        resultString.append("\n");
        resultString.append(imageString);
    }
}
//-----------------------------------------------------------------------------
void Converter::parseSimple(const QString &templateString,
                            QString &resultString,
                            QMap<QString, QString> &tags,
                            IDocument *doc)
{
    Q_UNUSED(doc);
    QRegExp regTag("@(.+)@");
    regTag.setMinimal(true);
    resultString = templateString;
    while (regTag.indexIn(resultString) >= 0)
    {
        QString tag = regTag.cap(0);
        QString tagName = regTag.cap(1);
        if (tags.contains(tagName))
            resultString.replace(tag, tags.value(tagName));
        else
            resultString.replace(tag, "<value not defined>");
    }
}
//-----------------------------------------------------------------------------
QString Converter::hexCode(const QChar &ch, const QString &encoding, bool bom)
{
    QString result;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toAscii());

    QByteArray codeArray = codec->fromUnicode(&ch, 1);

    quint64 code = 0;
    for (int i = 0; i < codeArray.count() && i < 8; i++)
    {
        code = code << 8;
        code |= (quint8)codeArray.at(i);
    }

    if (encoding.contains("UTF-16"))
    {
        if (bom)
        {
            // 0xfeff00c1
            result = QString("%1").arg(code, 8, 16, QChar('0'));
        }
        else
        {
            // 0x00c1
            code &= 0x000000000000ffff;
            result = QString("%1").arg(code, 4, 16, QChar('0'));
        }
    }
    else if (encoding.contains("UTF-32"))
    {
        if (bom)
        {
            // 0x0000feff000000c1
            result = QString("%1").arg(code, 16, 16, QChar('0'));
        }
        else
        {
            // 0x000000c1
            code &= 0x00000000ffffffff;
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
void Converter::addOrderInfo(QMap<QString, QString> &tags)
{
    if (this->mMatrix->options()->bytesOrder() == BytesOrderLittleEndian)
        tags.insert("bytesOrder", "little-endian");
    else
        tags.insert("bytesOrder", "big-endian");
}
//-----------------------------------------------------------------------------
void Converter::addPreprocessInfo(QMap<QString, QString> &tags)
{
    switch (this->mMatrix->options()->rotate())
    {
        case Rotate90:
            tags.insert("rotate", "90 degrees");
            break;
        case Rotate180:
            tags.insert("rotate", "180 degrees");
            break;
        case Rotate270:
            tags.insert("rotate", "270 degrees");
            break;
        case RotateNone:
            tags.insert("rotate", "none");
            break;
    }

    if (this->mMatrix->options()->flipHorizontal())
        tags.insert("flipHorizontal", "yes");
    else
        tags.insert("flipHorizontal", "no");

    if (this->mMatrix->options()->flipVertical())
        tags.insert("flipVertical", "yes");
    else
        tags.insert("flipVertical", "no");

    if (this->mMatrix->options()->inverse())
        tags.insert("inverse", "yes");
    else
        tags.insert("inverse", "no");
}
//-----------------------------------------------------------------------------
