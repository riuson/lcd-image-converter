#include "cmdlineparser.h"
#include "convertimagearguments.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
CmdLineParser::CmdLineParser(const QStringList &args)
{
    this->mArguments = &args;
    this->mParser = new QCommandLineParser();

    this->initializeParser(this->mParser);
}
//-----------------------------------------------------------------------------
CmdLineParser::~CmdLineParser()
{
    delete this->mParser;
}
//-----------------------------------------------------------------------------
const QObject *CmdLineParser::parsedArguments() const
{
    return this->mParsedArguments;
}
//-----------------------------------------------------------------------------
void CmdLineParser::initializeParser(QCommandLineParser *parser)
{
    this->mParsedArguments = NULL;

    parser->setApplicationDescription("Tool to create image and font source files for embedded applications.");
    parser->addHelpOption();
    parser->addVersionOption();

    // --mode=convert-image
    QCommandLineOption optionMode(QStringList() << "m" << "mode",
                QCoreApplication::translate("CmdLineParser", "Conversion mode for application, \"convert-image\"."),
                QCoreApplication::translate("CmdLineParser", "mode"));
    parser->addOption(optionMode);

    parser->parse(*this->mArguments);

    if (parser->value(optionMode) == ConvertImageArguments::modeName())
    {
        ConvertImageArguments::fillParser(parser);
    }

    // process arguments
    parser->process(*this->mArguments);

    if (parser->value(optionMode) == ConvertImageArguments::modeName())
    {
        this->mParsedArguments = new ConvertImageArguments(parser);
    }
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
