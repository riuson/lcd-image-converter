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

#ifndef SETUPTABTEMPLATES_H
#define SETUPTABTEMPLATES_H

#include <QWidget>

namespace Ui
{
class SetupTabTemplates;
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
namespace Templates
{

class SetupTabTemplates : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabTemplates(Settings::Presets::Preset *preset, QWidget *parent = 0);
  virtual ~SetupTabTemplates();

private:
  Ui::SetupTabTemplates *ui;
  Settings::Presets::Preset *mPreset;

private slots:
  void matrixChanged();
  void on_pushButtonBrowseImage_clicked();
  void on_pushButtonBrowseFont_clicked();
  void on_lineEditImage_editingFinished();
  void on_lineEditFont_editingFinished();
};

} // namespace Templates
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABTEMPLATES_H
