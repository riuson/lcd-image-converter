#ifndef CONVERTER_H
#define CONVERTER_H
//-----------------------------------------------------------------------------
#include "iconverter.h"

#include <QObject>
#include <QMap>
//-----------------------------------------------------------------------------
class Converter : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    Converter(QObject *parent);
    ~Converter();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);
    QString dataToString(const BitmapData &data);

    QStringList names() const ;
    QWidget *widgetSetup();
    IConverter *conv(const QString &name);
    void selectConv(const QString &name);
private:
    QMap <QString, QObject *> mConverters;
    QString mSelectedConverterName;
};
//-----------------------------------------------------------------------------
#endif // CONVERTER_H
