#include "modeconvertimage.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "preset.h"
#include <QCommandLineParser>
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QImage>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
ModeConvertImage::ModeConvertImage(QCommandLineParser *parser, QObject *parent) :
    ModeParserBase(parser, parent)
{
}
//-----------------------------------------------------------------------------
QString ModeConvertImage::modeName()
{
    return "convert-image";
}
//-----------------------------------------------------------------------------
void ModeConvertImage::fillParser() const
{
    // --input=/temp/1.png
    QCommandLineOption inputOption(QStringList() << "i" << "input",
                QCoreApplication::translate("CmdLineParser", "Full <path> to source image in binary format."),
                QCoreApplication::translate("CmdLineParser", "path"));
    this->mParser->addOption(inputOption);

    // --output=/temp/1.c
    QCommandLineOption outputOption(QStringList() << "o" << "output",
                QCoreApplication::translate("CmdLineParser", "Full <path> to output result."),
                QCoreApplication::translate("CmdLineParser", "path"));
    this->mParser->addOption(outputOption);

    // --template=/temp/image.tmpl
    QCommandLineOption templateOption(QStringList() << "template",
                QCoreApplication::translate("CmdLineParser", "Full <path> to template file, used in conversion."),
                QCoreApplication::translate("CmdLineParser", "path"));
    this->mParser->addOption(templateOption);

    // --doc-name=testImage
    QCommandLineOption documentNameOption(QStringList() << "doc-name",
                QCoreApplication::translate("CmdLineParser", "Document name."),
                QCoreApplication::translate("CmdLineParser", "name"));
    this->mParser->addOption(documentNameOption);

    // --preset-name=lcdR4G5B4
    QCommandLineOption presetOption(QStringList() << "preset-name",
                QCoreApplication::translate("CmdLineParser", "Output preset <name> from predefined presets in application settings."),
                QCoreApplication::translate("CmdLineParser", "name"));
    this->mParser->addOption(presetOption);
}
//-----------------------------------------------------------------------------
bool ModeConvertImage::collectArguments()
{
    this->mInputFilename = this->mParser->value("input");
    this->mOuputFilename = this->mParser->value("output");
    this->mTemplateFilename = this->mParser->value("template");
    this->mDocumentName = this->mParser->value("doc-name");
    this->mPresetName = this->mParser->value("preset-name");

    return (!this->mInputFilename.isEmpty() &&
            !this->mOuputFilename.isEmpty() &&
            !this->mTemplateFilename.isEmpty() &&
            !this->mDocumentName.isEmpty() &&
            !this->mPresetName.isEmpty());
}
//-----------------------------------------------------------------------------
int ModeConvertImage::process()
{
    if (QFile::exists(this->mInputFilename) && QFile::exists(this->mTemplateFilename))
    {
        QString docNameWS = this->mDocumentName;
        docNameWS.remove(QRegExp("\\s"));
        if (!docNameWS.isEmpty())
        {
            if (Preset::presetsList().contains(this->mPresetName))
            {
                Preset::setCurrentName(this->mPresetName);

                QImage imageLoaded;
                if (imageLoaded.load(this->mInputFilename))
                {
                    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

                    ImageDocument imageDocument;
                    QStringList keys = imageDocument.dataContainer()->keys();

                    QStringListIterator iterator(keys);
                    while (iterator.hasNext())
                    {
                        QString key = iterator.next();
                        imageDocument.dataContainer()->setImage(key, &imageConverted);

                        imageDocument.setDocumentName(docNameWS);
                        imageDocument.dataContainer()->setInfo("converted filename", QVariant(this->mOuputFilename));

                        imageDocument.convert(false);

                    }

                    return 0;
                }
            }
        }
    }

    return 1;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
