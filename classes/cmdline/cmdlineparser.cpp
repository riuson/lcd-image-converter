#include "cmdlineparser.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QDebug>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
CmdLineParser::CmdLineParser(const QCoreApplication *app)
{
    this->mApp = app;
    this->mParser = new QCommandLineParser();

    this->initializeParser(this->mParser);
}
//-----------------------------------------------------------------------------
CmdLineParser::~CmdLineParser()
{
    delete this->mParser;
}
//-----------------------------------------------------------------------------
Mode CmdLineParser::mode() const
{
    QString value = this->mParser->value("mode");

    if (value == "convert")
    {
        return ModeConvert;
    }

    return ModeUndefined;
}
//-----------------------------------------------------------------------------
DocumentType CmdLineParser::documentType() const
{
    QString value = this->mParser->value("type");

    if (value == "image")
    {
        return DocumentTypeImage;
    }
    else if (value == "font")
    {
        return DocumentTypeFont;
    }

    return DocumentTypeUndefined;
}
//-----------------------------------------------------------------------------
void CmdLineParser::initializeParser(QCommandLineParser *parser)
{
    parser->setApplicationDescription("Tool to create image and font source files for embedded applications.");
    parser->addHelpOption();
    parser->addVersionOption();

    // --mode=convert
    QCommandLineOption optionMode(QStringList() << "m" << "mode",
                QCoreApplication::translate("CmdLineParser", "Conversion mode for application, \"convert\"."),
                QCoreApplication::translate("CmdLineParser", "mode"));
    parser->addOption(optionMode);

    // --type=image
    QCommandLineOption typeOption(QStringList() << "t" << "type",
                QCoreApplication::translate("CmdLineParser", "Type of document, \"image\"."),
                QCoreApplication::translate("CmdLineParser", "type"));
    parser->addOption(typeOption);

    // --input=/temp/1.png
    QCommandLineOption inputOption(QStringList() << "i" << "input",
                QCoreApplication::translate("CmdLineParser", "Full <path> to source image in binary format."),
                QCoreApplication::translate("CmdLineParser", "path"));
    parser->addOption(inputOption);

    // --output=/temp/1.c
    QCommandLineOption outputOption(QStringList() << "o" << "output",
                QCoreApplication::translate("CmdLineParser", "Full <path> to output result."),
                QCoreApplication::translate("CmdLineParser", "path"));
    parser->addOption(outputOption);

    // --doc-name=testImage
    QCommandLineOption documentNameOption(QStringList() << "doc-name",
                QCoreApplication::translate("CmdLineParser", "Document name."),
                QCoreApplication::translate("CmdLineParser", "name"));
    parser->addOption(documentNameOption);

    // --preset=lcdR4G5B4
    QCommandLineOption presetOption(QStringList() << "preset",
                QCoreApplication::translate("CmdLineParser", "Output preset <name> from predefined presets in application settings."),
                QCoreApplication::translate("CmdLineParser", "name"));
    parser->addOption(presetOption);

    // --template=/temp/image.tmpl
    QCommandLineOption templateOption(QStringList() << "template",
                QCoreApplication::translate("CmdLineParser", "Template <filename>, used in conversion."),
                QCoreApplication::translate("CmdLineParser", "filename"));
    parser->addOption(templateOption);

    // process arguments
    parser->process(*this->mApp);
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
