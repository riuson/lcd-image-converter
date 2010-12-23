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

    QString outputString;

    QFile file(templateFile);
    //if (!file.exists())
    //{
    //    file.setFileName(":/templates/image_convert");
    //}
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        outputString = stream.readAll();
        file.close();
    }
    tags.insert("@templateFile@", file.fileName());

    QRegExp regEnc("@use_char_encoding.+@");
    regEnc.setMinimal(true);
    if (regEnc.indexIn(outputString) >= 0)
    {
        int index1 = regEnc.cap(0).indexOf("(");
        int index2 = regEnc.cap(0).indexOf(")");
        QString enc = regEnc.cap(0).mid(index1 + 1, index2 - index1 - 1);
        tags["@encoding@"] = enc;
        outputString.replace(regEnc, enc);
    }

    this->substHeader(outputString, tags);

    QRegExp regImagesTable("@start_images_table.*@end_images_table");
    regImagesTable.setMinimal(true);

    this->substImagesTable(outputString, tags, document->dataContainer());
    this->substCharsTable(outputString, tags, document->dataContainer());

    this->substTags(outputString, tags);

    result = outputString;

    return result;
}
//-----------------------------------------------------------------------------
void Converter::substHeader(QString &outputString, QMap<QString, QString> &tags)
{
    QRegExp regHeader("@start_header.*@end_header");
    if(regHeader.indexIn(outputString) >= 0)
    {
        QString header = regHeader.cap(0);
        header.remove("@start_header");
        header.remove("@end_header");
        this->substTags(header, tags);

        outputString.replace(regHeader, header);
    }
}
//-----------------------------------------------------------------------------
void Converter::substImagesTable(QString &outputString, QMap<QString, QString> &tags, IDataContainer *data)
{
    QRegExp regImagesTable("@start_images_table.*@end_images_table");
    regImagesTable.setMinimal(true);
    if(regImagesTable.indexIn(outputString) >= 0)
    {
        tags["@imagesCount@"] = QString("%1").arg(data->count());
        QString im = regImagesTable.cap(0);
        im.remove("@start_images_table");
        im.remove("@end_images_table");
        QRegExp regTag("@start_image.+@end_image");
        regTag.setMinimal(true);
        int index = 0;
        QString strImagesTable;
        if (regTag.indexIn(im) >= 0)
        {
            QString enc = "UTF-8";
            if (tags.contains("@encoding@"))
                enc = tags.value("@encoding@");

            QTextCodec *codec = QTextCodec::codecForName(enc.toAscii());

            QString imageTemplate = regTag.cap(0);
            QListIterator<QString> it(data->keys());
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                QByteArray codeArray = codec->fromUnicode(key);
                bool ok;

                quint32 code = (quint8)codeArray.at(0);
                tags["@charCode@"] = QString("%1").arg((ulong)code, 2, 16, QChar('0'));

                if (codeArray.length() >= 2)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(1);
                    tags["@charCode@"] = QString("%1").arg((ulong)code, 4, 16, QChar('0'));
                }
                if (codeArray.length() >= 3)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(2);
                    tags["@charCode@"] = QString("%1").arg((ulong)code, 6, 16, QChar('0'));
                }
                if (codeArray.length() >= 4)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(3);
                    tags["@charCode@"] = QString("%1").arg((ulong)code, 8, 16, QChar('0'));
                }


                QImage image = QImage(*data->image(key));
                image = this->preprocessImage(image);
                BitmapData bmpData;
                this->processImage(image, &bmpData);
                QString dataString = this->dataToString(bmpData);

                tags["@imageData@"] = dataString;
                tags["@blocksCount@"] = QString("%1").arg(bmpData.blocksCount());
                tags["@width@"] = QString("%1").arg(image.width());
                tags["@height@"] = QString("%1").arg(image.height());

                QString imageStr = imageTemplate;
                this->substImage(imageStr, tags);
                strImagesTable.append(imageStr);
            }
            //this->substTag(im, tags, tag);
        }
        outputString.replace(regImagesTable, strImagesTable);
    }
}
//-----------------------------------------------------------------------------
void Converter::substCharsTable(QString &outputString, QMap<QString, QString> &tags, IDataContainer *data)
{
    QRegExp regCharsTable("@start_chars_table.*@end_chars_table");
    regCharsTable.setMinimal(true);
    if(regCharsTable.indexIn(outputString) >= 0)
    {
        tags["@imagesCount@"] = QString("%1").arg(data->count());
        QString im = regCharsTable.cap(0);
        im.remove("@start_chars_table");
        im.remove("@end_chars_table");
        QRegExp regTag("@start_image.+@end_image");
        regTag.setMinimal(true);
        int index = 0;
        QString strImagesTable;
        if (regTag.indexIn(im) >= 0)
        {
            QString enc = "UTF-8";
            if (tags.contains("@encoding@"))
                enc = tags.value("@encoding@");

            QTextCodec *codec = QTextCodec::codecForName(enc.toAscii());

            QString imageTemplate = regTag.cap(0);
            QListIterator<QString> it(data->keys());
            it.toFront();
            while (it.hasNext())
            {
                QString key = it.next();
                QByteArray codeArray = codec->fromUnicode(key);
                bool ok;

                quint32 code = (quint8)codeArray.at(0);
                tags["@charCode@"] = QString("%1").arg(code, 2, 16, QChar('0'));

                if (codeArray.length() >= 2)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(1);
                    tags["@charCode@"] = QString("%1").arg(code, 4, 16, QChar('0'));
                }
                if (codeArray.length() >= 3)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(2);
                    tags["@charCode@"] = QString("%1").arg(code, 6, 16, QChar('0'));
                }
                if (codeArray.length() >= 4)
                {
                    code = code << 8;
                    code |= (quint8)codeArray.at(3);
                    tags["@charCode@"] = QString("%1").arg(code, 8, 16, QChar('0'));
                }


                QImage image = QImage(*data->image(key));
                image = this->preprocessImage(image);
                BitmapData bmpData;
                this->processImage(image, &bmpData);
                QString dataString = this->dataToString(bmpData);

                tags["@imageData@"] = dataString;
                tags["@blocksCount@"] = QString("%1").arg(bmpData.blocksCount());
                tags["@width@"] = QString("%1").arg(image.width());
                tags["@height@"] = QString("%1").arg(image.height());
                tags["@charText@"] = key;

                QString imageStr = imageTemplate;
                if (it.hasNext())
                    tags["@comma@"] = ",";
                else
                    tags["@comma@"] = "";
                this->substImage(imageStr, tags);
                strImagesTable.append(imageStr);
            }
            //this->substTag(im, tags, tag);
        }
        outputString.replace(regCharsTable, strImagesTable);
    }
}
//-----------------------------------------------------------------------------
void Converter::substImage(QString &outputString, QMap<QString, QString> &tags)
{
    QRegExp regImage("@start_image.*@end_image");
    regImage.setMinimal(true);
    if(regImage.indexIn(outputString) >= 0)
    {
        QString im = regImage.cap(0);
        im.remove("@start_image");
        im.remove("@end_image");
        QRegExp regTag("@.+@");
        regTag.setMinimal(true);
        int index = 0;
        while ((index = regTag.indexIn(im)) >= 0)
        {
            QString tag = regTag.cap(0);
            this->substTag(im, tags, tag);
        }

        outputString.replace(regImage, im);
    }

}
//-----------------------------------------------------------------------------
void Converter::substTags(QString &outputString, QMap<QString, QString> &tags)
{
    QRegExp regTag("@.+@");
    regTag.setMinimal(true);
    while (regTag.indexIn(outputString) >= 0)
    {
        QString tag = regTag.cap(0);
        this->substTag(outputString, tags, tag);
    }
}
//-----------------------------------------------------------------------------
void Converter::substTag(QString &outputString, QMap<QString, QString> &tags, const QString &tagName)
{
    if (outputString.contains(tagName))
    {
        if (tags.contains(tagName))
            outputString.replace(tagName, tags.value(tagName));
        else
            outputString.replace(tagName, "<value not defined>");
    }
}
//-----------------------------------------------------------------------------
