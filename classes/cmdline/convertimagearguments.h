#ifndef CONVERTIMAGEARGUMENTS_H
#define CONVERTIMAGEARGUMENTS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QCommandLineParser;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class ConvertImageArguments : public QObject
{
    Q_OBJECT
public:
    explicit ConvertImageArguments(QCommandLineParser *parser, QObject *parent = 0);

    static void fillParser(QCommandLineParser *parser);
    static QString modeName();

    QString inputFilename() const;
    QString outputFilename() const;
    QString templateFilename() const;
    QString documentName() const;
    QString presetName() const;

private:
    QString mInputFilename;
    QString mOuputFilename;
    QString mTemplateFilename;
    QString mDocumentName;
    QString mPresetName;
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CONVERTIMAGEARGUMENTS_H
