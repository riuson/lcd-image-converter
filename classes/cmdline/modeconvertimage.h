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

#ifndef CONVERTIMAGEARGUMENTS_H
#define CONVERTIMAGEARGUMENTS_H

#include <QObject>

#include "modeparserbase.h"

namespace CommandLine
{

class ModeConvertImage : public ModeParserBase
{
  Q_OBJECT
public:
  explicit ModeConvertImage(QCommandLineParser& parser, QObject* parent = 0);
  virtual ~ModeConvertImage() {}

  static QString modeName();

  void fillParser() const Q_DECL_OVERRIDE;
  bool collectArguments() Q_DECL_OVERRIDE;
  int process() Q_DECL_OVERRIDE;

private:
  QString mInputFilename;
  QString mOutputFilename;
  QString mTemplateFilename;
  QString mDocumentName;
  QString mPresetName;
};

} // namespace CommandLine

#endif // CONVERTIMAGEARGUMENTS_H
