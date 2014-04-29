#include "cmdline.h"
#include "cmdlineparser.h"
#include "preset.h"
#include "parser.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "convertimagearguments.h"
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
    if (this->mParser->parsedArguments() != NULL)
    {
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
int CmdLine::process()
{
    if (this->mParser->parsedArguments() != NULL)
    {
        const ConvertImageArguments *arguments = qobject_cast<const ConvertImageArguments *>(this->mParser->parsedArguments());
        if (arguments != NULL)
        {
            QString inputFilename = arguments->inputFilename();
            QString outputFilename = arguments->outputFilename();
            QString templateFilename = arguments->templateFilename();
            QString presetName = arguments->presetName();
            QString documentName = arguments->documentName();

            return this->convertImage(inputFilename, outputFilename, templateFilename, presetName, documentName);
        }
    }

    return 1;
}
//-----------------------------------------------------------------------------
int CmdLine::convertImage(const QString &inputFilename, const QString &outputFilename, const QString &templateFilename, const QString &presetName, const QString &documentName)
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

                        return 0;
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
