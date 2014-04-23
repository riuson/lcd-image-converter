#ifndef CMDLINEPARSER_H
#define CMDLINEPARSER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QCommandLineParser;
class QCoreApplication;
//-----------------------------------------------------------------------------
class CmdLineParser
{
public:
    CmdLineParser(QCoreApplication *app);

    bool hasArguments() const;
    int process();

private:
    const QCoreApplication *mApp;

    void initializeParser(QCommandLineParser *parser);
};
//-----------------------------------------------------------------------------
#endif // CMDLINEPARSER_H
