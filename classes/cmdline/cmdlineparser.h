#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H
//-----------------------------------------------------------------------------
#include "cmdoptions.h"
#include <QObject>
//-----------------------------------------------------------------------------
class QCommandLineParser;
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class CmdLineParser
{
public:
    CmdLineParser(const QStringList &args);
    ~CmdLineParser();

    const QObject *parsedArguments() const;

private:
    const QStringList *mArguments;
    QCommandLineParser *mParser;
    QObject *mParsedArguments;

    void initializeParser(QCommandLineParser *parser);
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CMDLINEPARSER_H
