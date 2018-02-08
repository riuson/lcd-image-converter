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

#ifndef CONVERTFONTARGUMENTS_H
#define CONVERTFONTARGUMENTS_H

#include "modeparserbase.h"
#include <QObject>

namespace CommandLine
{

class ModeConvertFont : public ModeParserBase
{
  Q_OBJECT
public:
  explicit ModeConvertFont(QCommandLineParser &parser, QObject *parent = 0);
  virtual ~ModeConvertFont() {}

  static QString modeName();

  void fillParser() const Q_DECL_OVERRIDE;
  bool collectArguments() Q_DECL_OVERRIDE;
  int process() Q_DECL_OVERRIDE;

private:
  enum class SubMode {
    None,
    FromXmlDocument,
    FromCharactersList,
    FromCharactersRange
  };

  QString mFontFamily;
  int mFontSize;
  bool mFontMonospaced;
  QString mFontStyle;
  bool mFontAntiAliasing;
  QString mForeground;
  QString mBackground;
  int mMultiplicityHeight;
  int mMultiplicityWidth;

  QString mFontCharactersList;

  QString mFontCharactersEncoding;
  QString mFontCharactersRange;
  bool mFontCharactersBigEndian;

  QString mInputFilename;
  QString mOutputFilename;
  QString mTemplateFilename;
  QString mDocumentName;
  QString mDocumentNameWS;
  QString mPresetName;

  SubMode mSubMode;

  QString createCharsList(const QString &rangeStr,
                          const QString &encoding,
                          bool bigEndian) const;
};

} // namespace CommandLine

#endif // CONVERTFONTARGUMENTS_H
