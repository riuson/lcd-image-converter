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

#ifndef SETUPTABIMAGE_H
#define SETUPTABIMAGE_H

#include <QWidget>

namespace Ui
{
class SetupTabImage;
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
namespace Image
{

class SetupTabImage : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabImage(Settings::Presets::Preset *preset, QWidget *parent = 0);
  virtual ~SetupTabImage();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabImage *ui;
  Settings::Presets::Preset *mPreset;

private slots:
  void on_checkBoxSplitToRows_toggled(bool value);
  void on_radioButtonLittleEndian_toggled(bool value);
  void on_comboBoxBlockSize_currentIndexChanged(int index);
  void on_checkBoxCompressionRle_toggled(bool value);
  void on_spinBoxRleMinLength_valueChanged(int value);
  void on_checkBoxBlockDefaultOnes_toggled(bool value);
  void on_lineEditBlockPrefix_textEdited(const QString &value);
  void on_lineEditBlockSuffix_textEdited(const QString &value);
  void on_lineEditBlockDelimiter_textEdited(const QString &value);
  void on_lineEditPreviewPrefix_textEdited(const QString &value);
  void on_lineEditPreviewSuffix_textEdited(const QString &value);
  void on_lineEditPreviewDelimiter_textEdited(const QString &value);
  void on_textEditPreviewLevels_textChanged();
};

} // namespace Image
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABIMAGE_H
