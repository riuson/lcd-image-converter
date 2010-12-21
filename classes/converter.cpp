#include "converter.h"
//-----------------------------------------------------------------------------
#include <QSettings>

#include "widgetconvoptionscolor.h"
#include "widgetconvoptionsgray.h"
#include "widgetconvoptionsmono.h"

#include "convertercolor.h"
#include "convertergrayscale.h"
#include "convertermono.h"
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
