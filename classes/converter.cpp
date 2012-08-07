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

#include "widgetconvoptionscolor.h"
#include "widgetconvoptionsgray.h"
#include "widgetconvoptionsmono.h"

#include "convertercolor.h"
#include "convertergrayscale.h"
#include "convertermono.h"

#include "bitmapdata.h"
#include "idocument.h"
#include "idatacontainer.h"
#include "bitmaphelper.h"
#include <QDebug>
//-----------------------------------------------------------------------------
Converter::Converter(QObject *parent) :
        QObject(parent)
{
    ConverterMono *mono = new ConverterMono(this);
    ConverterGrayscale *gray = new ConverterGrayscale(this);
    ConverterColor *color = new ConverterColor(this);

    this->mConverters.insert(mono->name(), mono);
    this->mConverters.insert(gray->name(), gray);
    this->mConverters.insert(color->name(), color);
    this->mSelectedConverterName = mono->name();

    this->mPreprocessTransform = TransformNone;

    this->loadSettings();
}
//-----------------------------------------------------------------------------
Converter::~Converter()
{
    this->saveSettings();
    qDeleteAll(this->mConverters);
    this->mConverters.clear();
}
//-----------------------------------------------------------------------------
void Converter::loadSettings()
{
    QSettings sett;
    sett.beginGroup("converters");

    QString name = sett.value("selected", this->mSelectedConverterName).toString();
    this->mSelectedConverterName = name;

    bool ok;
    this->mPreprocessTransform = sett.value("transform", QVariant(0)).toInt(&ok);
    if (!ok)
        this->mPreprocessTransform = 0;

    sett.endGroup();

    QListIterator<QString> it(this->mConverters.keys());
    it.toFront();
    while (it.hasNext())
    {
        QString key = it.next();
        IConverter *conv = dynamic_cast<IConverter *>(this->mConverters.value(key));
        conv->loadSettings();
    }
}
//-----------------------------------------------------------------------------
void Converter::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");

    sett.setValue("selected", this->mSelectedConverterName);

    sett.setValue("transform", this->mPreprocessTransform);

    sett.endGroup();

    QListIterator<QString> it(this->mConverters.keys());
    it.toFront();
    while (it.hasNext())
    {
        QString key = it.next();
        IConverter *conv = dynamic_cast<IConverter *>(this->mConverters.value(key));
        conv->saveSettings();
    }
}
//-----------------------------------------------------------------------------
QString Converter::name()
{
    //IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    //return options->name();
    return this->mSelectedConverterName;
}
//-----------------------------------------------------------------------------
QString Converter::displayName()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->displayName();
}
//-----------------------------------------------------------------------------
QImage Converter::preprocessImage(const QImage &source)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    QImage result = options->preprocessImage(source);

    if ((this->mPreprocessTransform & 0x03) == TransformRotate90)
        result = BitmapHelper::rotate90(&result);

    if ((this->mPreprocessTransform & 0x03) == TransformRotate180)
        result = BitmapHelper::rotate180(&result);

    if ((this->mPreprocessTransform & 0x03) == TransformRotate270)
        result = BitmapHelper::rotate270(&result);

    if ((this->mPreprocessTransform & TransformFlipHorizontal) == TransformFlipHorizontal)
        result = BitmapHelper::flipHorizontal(&result);

    if ((this->mPreprocessTransform & TransformFlipVertical) == TransformFlipVertical)
        result = BitmapHelper::flipVertical(&result);

    if ((this->mPreprocessTransform & TransformInverse) == TransformInverse)
        result.invertPixels();

    return result;
}
//-----------------------------------------------------------------------------
void Converter::processImage(const QImage &preprocessedImage, BitmapData *output)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->processImage(preprocessedImage, output);
}
//-----------------------------------------------------------------------------
QString Converter::dataToString(const BitmapData &data)
{
    QString result;
    int bits = data.blockSize();
    for (int line = 0, counter = 0; line < data.height(); line++)
    {
        if (result.endsWith(", "))
            result.append("\n");
        for (int column = 0; column < data.blocksPerLine(); column++)
        {
            quint32 value = data.data()->at(counter++);
            result.append(QString("0x%1, ").arg(value, bits / 4, 16, QChar('0')));
        }
    }
    if (result.endsWith(", "))
        result = result.remove(QRegExp("\\,\\s$"));
    return result;
}
//-----------------------------------------------------------------------------
bool Converter::swapBytes()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->swapBytes();
}
//-----------------------------------------------------------------------------
IConverter::DataLength Converter::length()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->length();
}
//-----------------------------------------------------------------------------
bool Converter::mirror()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->mirror();
}
//-----------------------------------------------------------------------------
bool Converter::pack()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->pack();
}
//-----------------------------------------------------------------------------
IConverter::DataAlign Converter::align()
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    return options->align();
}
//-----------------------------------------------------------------------------
void Converter::setSwapBytes(bool value)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->setSwapBytes(value);
}
//-----------------------------------------------------------------------------
void Converter::setLength(DataLength value)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->setLength(value);
}
//-----------------------------------------------------------------------------
void Converter::setMirror(bool value)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->setMirror(value);
}
//-----------------------------------------------------------------------------
void Converter::setPack(bool value)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->setPack(value);
}
//-----------------------------------------------------------------------------
void Converter::setAlign(DataAlign value)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    options->setAlign(value);
}
//-----------------------------------------------------------------------------
QStringList Converter::names() const
{
    QStringList result(this->mConverters.keys());
    return result;
}
//-----------------------------------------------------------------------------
QWidget *Converter::widgetSetup()
{
    QWidget *w = NULL;

    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(this->mSelectedConverterName));
    if (options)
    {
        if (options->name() == "color")
            w = new WidgetConvOptionsColor(options);
        if (options->name() == "grayscale")
            w = new WidgetConvOptionsGray(options);
        if (options->name() == "mono")
            w = new WidgetConvOptionsMono(options);
    }
    return w;
}
//-----------------------------------------------------------------------------
IConverter *Converter::conv(const QString &name)
{
    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(name));
    return options;
}
//-----------------------------------------------------------------------------
void Converter::selectConv(const QString &name)
{
    this->mSelectedConverterName = name;
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
int Converter::transform()
{
    return this->mPreprocessTransform;
}
//-----------------------------------------------------------------------------
void Converter::setTransform(int value)
{
    this->mPreprocessTransform = value;
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
            qDebug() << resultString;
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
                qDebug() << temp;
                resultString.append(temp);
                qDebug() << resultString;

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
        qDebug() << resultString;
    }
    int last = prevIndex;
    if (last < templateString.length() - 1)
    {
        resultString.append(templateString.mid(last, templateString.length() - last));
    }
    qDebug() << resultString;
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

            qDebug() << content;
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

        image = this->preprocessImage(image);
        BitmapData bmpData;
        this->processImage(image, &bmpData);
        QString dataString = this->dataToString(bmpData);

        bool useBom = false;
        if (tags.value("bom") == "YES")
            useBom = true;

        QString charCode = this->hexCode(key.at(0), tags.value("encoding"), useBom);

        tags["blocksCount"] = QString("%1").arg(bmpData.blocksCount());
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
    if (this->swapBytes())
        tags.insert("swapBytes", "yes");
    else
        tags.insert("swapBytes", "no");

    if (this->mirror())
        tags.insert("mirror", "yes");
    else
        tags.insert("mirror", "no");

    if (this->pack())
        tags.insert("pack", "yes");
    else
        tags.insert("pack", "no");
}
//-----------------------------------------------------------------------------
void Converter::addPreprocessInfo(QMap<QString, QString> &tags)
{
    switch (this->mPreprocessTransform & 0x03)
    {
        case TransformRotate90:
            tags.insert("rotate", "90 degrees");
            break;
        case TransformRotate180:
            tags.insert("rotate", "180 degrees");
            break;
        case TransformRotate270:
            tags.insert("rotate", "270 degrees");
            break;
        case TransformNone:
            tags.insert("rotate", "none");
            break;
    }

    if ((this->mPreprocessTransform & TransformFlipHorizontal) == TransformFlipHorizontal)
        tags.insert("flipHorizontal", "yes");
    else
        tags.insert("flipHorizontal", "no");

    if ((this->mPreprocessTransform & TransformFlipVertical) == TransformFlipVertical)
        tags.insert("flipVertical", "yes");
    else
        tags.insert("flipVertical", "no");

    if ((this->mPreprocessTransform & TransformInverse) == TransformInverse)
        tags.insert("inverse", "yes");
    else
        tags.insert("inverse", "no");
}
//-----------------------------------------------------------------------------
