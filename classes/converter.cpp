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

    sett.endGroup();
}
//-----------------------------------------------------------------------------
void Converter::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");

    sett.setValue("selected", this->mSelectedConverterName);

    sett.endGroup();
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
    return options->preprocessImage(source);
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
        tags["encoding"] = enc;
    }
    else
    {
        tags["encoding"] = "UTF-8";
    }

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
            int len = regTag.cap(0).length();
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
                this->parseSimple(content, contentParsed, tags, doc, 0);
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
        image = this->preprocessImage(image);
        BitmapData bmpData;
        this->processImage(image, &bmpData);
        QString dataString = this->dataToString(bmpData);
        QString charCode = this->hexCode(key.at(0), tags.value("encoding"));

        tags["blocksCount"] = QString("%1").arg(bmpData.blocksCount());
        tags["width"] = QString("%1").arg(image.width());
        tags["height"] = QString("%1").arg(image.height());
        tags["imageData"] = dataString;
        tags["charCode"] = charCode;
        tags["charText"] = key.left(1);
        if (it.hasNext())
            tags["comma"] = ",";
        else
            tags["comma"] = "";

        this->parseSimple(templateString, imageString, tags, doc, 0);
        resultString.append("\n");
        resultString.append(imageString);
    }
}
//-----------------------------------------------------------------------------
void Converter::parseSimple(const QString &templateString,
                            QString &resultString,
                            QMap<QString, QString> &tags,
                            IDocument *doc,
                            int startIndex = 0)
{
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
QString Converter::hexCode(const QChar &ch, const QString &encoding)
{
    QString result;
    QTextCodec *codec = QTextCodec::codecForName(encoding.toAscii());

    QByteArray codeArray = codec->fromUnicode(&ch, 1);

    quint32 code = 0;
    for (int i = 0; i < codeArray.count() && i < 4; i++)
    {
        code = code << 8;
        code |= (quint8)codeArray.at(i);
    }
    result = QString("%1").arg(code, codeArray.count() * 2, 16, QChar('0'));

    return result;
}
//-----------------------------------------------------------------------------
