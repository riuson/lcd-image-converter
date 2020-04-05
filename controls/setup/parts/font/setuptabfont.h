/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef SETUPTABFONT_H
#define SETUPTABFONT_H

#include <QWidget>

#include "conversion_options.h"

class QCompleter;

namespace Ui
{
class SetupTabFont;
}

namespace Settings
{
namespace Presets
{
class Preset;
}
}

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Font
{

class SetupTabFont : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabFont(Settings::Presets::Preset *preset, QWidget *parent = 0);
  virtual ~SetupTabFont();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabFont *ui;
  Settings::Presets::Preset *mPreset;
  QCompleter *mEncodingCompleter;

  const QString sortingName(Parsing::Conversion::Options::CharactersSortOrder value) const;

private slots:
  void on_checkBoxBom_toggled(bool value);
  void on_comboBoxEncoding_currentIndexChanged(const QString &value);
  void on_comboBoxSorting_currentIndexChanged(int index);
  void on_checkBoxSkipMissingCharacters_toggled(bool value);
  void on_lineEditEscapedCharacters_textEdited(const QString &value);
  void on_lineEditEscapePrefix_textEdited(const QString &value);
  void on_lineEditEscapeSuffix_textEdited(const QString &value);
};

} // namespace Font
} // namespace Font
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABFONT_H
