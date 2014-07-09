/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "qt-version-check.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
//-----------------------------------------------------------------------------
#include "cmdline.h"
#include "preset.h"
#include "parser.h"
#include "imagedocument.h"
#include "datacontainer.h"
#include "modeconvertimage.h"
#include "modehex2bin.h"
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
                QCoreApplication::translate("CmdLineParser", "Conversion mode for application, \"convert-image\" or \"hex2bin\"."),
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

    this->mParser->parse(*this->mArguments);

    QString modeName = this->mParser->value("mode");

    ModeParserBase *mode = this->createMode(modeName, this->mParser);

    if (mode != NULL)
    {
        mode->fillParser();
    }

    this->mParser->process(*this->mArguments);

    if (mode != NULL)
    {
        if (mode->collectArguments())
        {
            result = mode->process();
        }
        else
        {
            qDebug() << "Argumenths missing";
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

    if (name == ModeHex2Bin::modeName())
    {
        return new ModeHex2Bin(parser, this);
    }

    return NULL;
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // QT_VERSION
