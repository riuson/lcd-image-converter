#include "qt-version-check.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
//-----------------------------------------------------------------------------
#ifndef CMDLINE_H
#define CMDLINE_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QString;
class QStringList;
class QCommandLineParser;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class ModeParserBase;
//-----------------------------------------------------------------------------
class CmdLine : public QObject
{
    Q_OBJECT
public:
    explicit CmdLine(const QStringList &arguments, QObject *parent = 0);
    virtual ~CmdLine();

    bool needProcess() const;
    int process();
private:
    QCommandLineParser *mParser;
    const QStringList *mArguments;

    CommandLine::ModeParserBase *createMode(const QString &name, QCommandLineParser *parser);
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CMDLINE_H
//-----------------------------------------------------------------------------
#endif // QT_VERSION
