#include "cmdlineparser.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QDebug>
//-----------------------------------------------------------------------------
CmdLineParser::CmdLineParser(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
bool CmdLineParser::hasArguments() const
{
    QStringList arguments = QCoreApplication::arguments();
    return (arguments.length() > 1);
}
//-----------------------------------------------------------------------------
int CmdLineParser::process()
{
    return 1;
}
//-----------------------------------------------------------------------------
