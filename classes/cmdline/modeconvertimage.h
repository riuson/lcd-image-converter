#include <qt-version-check.h>

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
//-----------------------------------------------------------------------------
#ifndef CONVERTIMAGEARGUMENTS_H
#define CONVERTIMAGEARGUMENTS_H
//-----------------------------------------------------------------------------
#include "modeparserbase.h"
#include <QObject>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class ModeConvertImage : public ModeParserBase
{
    Q_OBJECT
public:
    explicit ModeConvertImage(QCommandLineParser *parser, QObject *parent = 0);

    static QString modeName();

    void fillParser() const;
    bool collectArguments();
    int process();

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
//-----------------------------------------------------------------------------
#endif // QT_VERSION
