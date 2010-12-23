#ifndef CONVERTER_H
#define CONVERTER_H
//-----------------------------------------------------------------------------
#include "iconverter.h"

#include <QObject>
#include <QMap>

class IDocument;
class IDataContainer;
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
    QString convert(IDocument *document, const QString &templateFile, QMap<QString, QString> &tags);
private:
    QMap <QString, QObject *> mConverters;
    QString mSelectedConverterName;
    void substHeader(QString &outputString, QMap<QString, QString> &tags);
    void substImagesTable(QString &outputString, QMap<QString, QString> &tags, IDataContainer *data);
    void substCharsTable(QString &outputString, QMap<QString, QString> &tags, IDataContainer *data);
    void substImage(QString &outputString, QMap<QString, QString> &tags);
    void substTags(QString &outputString, QMap<QString, QString> &tags);
    void substTag(QString &outputString, QMap<QString, QString> &tags, const QString &tagName);
};
//-----------------------------------------------------------------------------
#endif // CONVERTER_H
