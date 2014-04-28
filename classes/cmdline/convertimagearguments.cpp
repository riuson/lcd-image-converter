#include "convertimagearguments.h"
#include <QCommandLineParser>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
ConvertImageArguments::ConvertImageArguments(QCommandLineParser *parser, QObject *parent) :
    QObject(parent)
{
    this->mInputFilename = parser->value("input");
    this->mOuputFilename = parser->value("output");
    this->mTemplateFilename = parser->value("template");
    this->mDocumentName = parser->value("doc-name");
    this->mPresetName = parser->value("preset-name");
}
//-----------------------------------------------------------------------------
void ConvertImageArguments::fillParser(QCommandLineParser *parser)
{
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

    // --template=/temp/image.tmpl
    QCommandLineOption templateOption(QStringList() << "template",
                QCoreApplication::translate("CmdLineParser", "Full <path> to template file, used in conversion."),
                QCoreApplication::translate("CmdLineParser", "path"));
    parser->addOption(templateOption);

    // --doc-name=testImage
    QCommandLineOption documentNameOption(QStringList() << "doc-name",
                QCoreApplication::translate("CmdLineParser", "Document name."),
                QCoreApplication::translate("CmdLineParser", "name"));
    parser->addOption(documentNameOption);

    // --preset-name=lcdR4G5B4
    QCommandLineOption presetOption(QStringList() << "preset-name",
                QCoreApplication::translate("CmdLineParser", "Output preset <name> from predefined presets in application settings."),
                QCoreApplication::translate("CmdLineParser", "name"));
    parser->addOption(presetOption);
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::modeName()
{
    return "convert-image";
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::inputFilename() const
{
    return this->mInputFilename;
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::outputFilename() const
{
    return this->mOuputFilename;
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::templateFilename() const
{
    return this->mTemplateFilename;
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::documentName() const
{
    return this->mDocumentName;
}
//-----------------------------------------------------------------------------
QString ConvertImageArguments::presetName() const
{
    return this->mPresetName;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
