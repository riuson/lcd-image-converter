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

    void parse(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseBlocks(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseImagesTable(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseSimple(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc, int index);
    QString hexCode(const QChar &ch, const QString &encoding);
};
//-----------------------------------------------------------------------------
#endif // CONVERTER_H
