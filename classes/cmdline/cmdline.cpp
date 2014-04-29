#include "cmdline.h"
#include "preset.h"
#include "parser.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "modeconvertimage.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QImage>
#include <QCommandLineParser>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
CmdLine::CmdLine(const QStringList &arguments, QObject *parent) :
    QObject(parent)
{
    this->mArguments = new QStringList(arguments);
    this->mParser = new QCommandLineParser();

    // information block
    this->mParser->setApplicationDescription("Tool to create image and font source files for embedded applications.");
    this->mParser->addHelpOption();
    this->mParser->addVersionOption();

    // required option:
    // --mode=convert-image
    QCommandLineOption optionMode(QStringList() << "m" << "mode",
                QCoreApplication::translate("CmdLineParser", "Conversion mode for application, \"convert-image\"."),
                QCoreApplication::translate("CmdLineParser", "mode"));
    this->mParser->addOption(optionMode);
}
//-----------------------------------------------------------------------------
CmdLine::~CmdLine()
{
    delete this->mParser;
    delete this->mArguments;
}
//-----------------------------------------------------------------------------
bool CmdLine::needProcess() const
{
    if (this->mArguments->length() > 1)
    {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
int CmdLine::process()
{
    int result = 1;

    bool parsed = this->mParser->parse(*this->mArguments);

    QString modeName = this->mParser->value("mode");

    ModeParserBase *mode = this->createMode(modeName, this->mParser);

    if (mode != NULL)
    {
        mode->fillParser();
    }

    this->mParser->process(*this->mArguments);

    if (mode != NULL)
    {
        if (parsed)
        {
            if (mode->collectArguments())
            {
                result = mode->process();
            }
        }

        delete mode;
    }

    if (result != 0)
    {
        this->mParser->showHelp(1);
    }

    return result;
}
//-----------------------------------------------------------------------------
ModeParserBase *CmdLine::createMode(const QString &name, QCommandLineParser *parser)
{
    if (name == ModeConvertImage::modeName())
    {
        return new ModeConvertImage(parser, this);
    }

    return NULL;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
