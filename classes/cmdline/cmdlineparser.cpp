#include "cmdlineparser.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QDebug>
//-----------------------------------------------------------------------------
CmdLineParser::CmdLineParser(QCoreApplication *app)
{
    this->mApp = app;
}
//-----------------------------------------------------------------------------
bool CmdLineParser::hasArguments() const
{
    QStringList arguments = this->mApp->arguments();
    return (arguments.length() > 1);
}
//-----------------------------------------------------------------------------
int CmdLineParser::process()
{
    QCommandLineParser parser;
    this->initializeParser(&parser);

    return 1;
}
//-----------------------------------------------------------------------------
void CmdLineParser::initializeParser(QCommandLineParser *parser)
{
    parser->setApplicationDescription("Tool to create image and font source files for embedded applications.");
    parser->addHelpOption();
    parser->addVersionOption();

    parser->process(*this->mApp);
}
//-----------------------------------------------------------------------------
