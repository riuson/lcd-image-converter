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

#include <qt-version-check.h>

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)
//-----------------------------------------------------------------------------
#ifndef CONVERTFONTARGUMENTS_H
#define CONVERTFONTARGUMENTS_H
//-----------------------------------------------------------------------------
#include "modeparserbase.h"
#include <QObject>
//-----------------------------------------------------------------------------
namespace CommandLine {
//-----------------------------------------------------------------------------
class ModeConvertFont : public ModeParserBase
{
    Q_OBJECT
public:
    explicit ModeConvertFont(QCommandLineParser *parser, QObject *parent = 0);

    static QString modeName();

    void fillParser() const;
    bool collectArguments();
    int process();

private:
    QString mFontFamily;
    int mFontSize;
    bool mFontMonospaced;
    bool mFontBold;
    bool mFontItalic;
    bool mFontAntiAliasing;
    QString mFontCharactersList;

    QString mFontCharactersEncoding;
    QString mFontCharactersRange;

    QString mOuputFilename;
    QString mTemplateFilename;
    QString mDocumentName;
    QString mPresetName;
};
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
#endif // CONVERTFONTARGUMENTS_H
//-----------------------------------------------------------------------------
#endif // QT_VERSION
