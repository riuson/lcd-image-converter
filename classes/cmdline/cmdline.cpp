#include "cmdline.h"
#include "cmdlineparser.h"
#include "preset.h"
#include "parser.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include <QDebug>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QImage>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
CmdLine::CmdLine(const QStringList &arguments)
{
    this->mParser = new CmdLineParser(arguments);
}
//-----------------------------------------------------------------------------
CmdLine::~CmdLine()
{
    delete this->mParser;
}
//-----------------------------------------------------------------------------
bool CmdLine::needProcess() const
{
    if (this->mParser->mode() == ModeConvert)
    {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
int CmdLine::process()
{
    if (this->mParser->mode() == ModeConvert)
    {
        if (this->mParser->documentType() == DocumentTypeImage)
        {
            QString inputFilename = this->mParser->inputFilename();
            QString outputFilename = this->mParser->outputFilename();
            QString templateFilename = this->mParser->templateFilename();
            QString presetName = this->mParser->presetName();
            QString documentName = this->mParser->documentName();

            return this->convertBinaryImage(inputFilename, outputFilename, templateFilename, presetName, documentName);
        }
    }

    return 1;
}
//-----------------------------------------------------------------------------
int CmdLine::convertBinaryImage(const QString &inputFilename, const QString &outputFilename, const QString &templateFilename, const QString &presetName, const QString &documentName)
{
    if (QFile::exists(inputFilename) && QFile::exists(templateFilename))
    {
        QString docNameWS = documentName;
        docNameWS.remove(QRegExp("\\s"));
        if (!docNameWS.isEmpty())
        {
            if (Preset::presetsList().contains(presetName))
            {
                Preset::setCurrentName(presetName);

                QImage imageLoaded;
                if (imageLoaded.load(inputFilename))
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
                        imageDocument.dataContainer()->setInfo("converted filename", QVariant(outputFilename));

                        imageDocument.convert(false);
                    }
                }
            }
        }
    }

    return 1;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
