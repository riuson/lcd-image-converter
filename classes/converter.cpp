#include "converter.h"
//-----------------------------------------------------------------------------
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
    ConverterColor *color = new ConverterColor(this);
    ConverterGrayscale *gray = new ConverterGrayscale(this);
    ConverterMono *mono = new ConverterMono(this);

    this->mConverters.insert(color->name(), color);
    this->mConverters.insert(gray->name(), gray);
    this->mConverters.insert(mono->name(), mono);
}
//-----------------------------------------------------------------------------
Converter::~Converter()
{
    qDeleteAll(this->mConverters);
    this->mConverters.clear();
}
//-----------------------------------------------------------------------------
QStringList Converter::names() const
{
    QStringList result(this->mConverters.keys());
    return result;
}
//-----------------------------------------------------------------------------
QWidget *Converter::widgetSetup(const QString &name)
{
    QWidget *w = NULL;

    IConverter *options = dynamic_cast<IConverter *>(this->mConverters.value(name));
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
