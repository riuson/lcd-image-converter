#include "modeconvertimage.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "preset.h"
#include "templateoptions.h"
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
            !this->mDocumentName.isEmpty() &&
            !this->mPresetName.isEmpty());
}
//-----------------------------------------------------------------------------
int ModeConvertImage::process()
{
    // check input and template files exists
    if (QFile::exists(this->mInputFilename))
    {
        // check dodocument name
        QString docNameWS = this->mDocumentName;
        docNameWS.remove(QRegExp("\\s"));
        if (!docNameWS.isEmpty())
        {
            // check preset exists
            if (Preset::presetsList().contains(this->mPresetName))
            {
                Preset::setSelectedName(this->mPresetName);

                // load image from input file
                QImage imageLoaded;
                if (imageLoaded.load(this->mInputFilename))
                {
                    QImage imageConverted = imageLoaded.convertToFormat(QImage::Format_ARGB32);

                    ImageDocument imageDocument;
                    QStringList keys = imageDocument.dataContainer()->keys();

                    // process all keys (1 in image document)
                    QStringListIterator iterator(keys);
                    while (iterator.hasNext())
                    {
                        QString key = iterator.next();
                        imageDocument.dataContainer()->setImage(key, &imageConverted);

                        imageDocument.setDocumentName(docNameWS);
                        imageDocument.dataContainer()->setInfo("converted filename", QVariant(this->mOuputFilename));

                        // save to output file
                        QFile file(this->mOuputFilename);
                        if (file.open(QFile::WriteOnly))
                        {
                            Preset preset;
                            preset.load(Preset::selectedName());

                            // optional template file
                            if(!this->mTemplateFilename.isEmpty() && QFile::exists(this->mTemplateFilename))
                            {
                                preset.templates()->setImage(this->mTemplateFilename);
                            }

                            QString result = imageDocument.convert(&preset);

                            file.write(result.toUtf8());
                            file.close();

                            if (imageDocument.outputFilename() != this->mOuputFilename)
                            {
                                imageDocument.setOutputFilename(this->mOuputFilename);
                            }
                        }
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
