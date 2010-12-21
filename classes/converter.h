#ifndef CONVERTER_H
#define CONVERTER_H
//-----------------------------------------------------------------------------
#include "iconverter.h"

#include <QObject>
#include <QMap>
//-----------------------------------------------------------------------------
class Converter : public QObject
{
    Q_OBJECT
public:
    Converter(QObject *parent);
    ~Converter();

    QStringList names() const ;
    QWidget *widgetSetup(const QString &name);
    IConverter *conv(const QString &name);
private:
    QMap <QString, QObject *> mConverters;
};
//-----------------------------------------------------------------------------
#endif // CONVERTER_H
