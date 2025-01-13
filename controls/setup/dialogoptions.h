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

#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H

#include <QDialog>

namespace Ui
{
class DialogOptions;
}

namespace Data
{
namespace Containers
{
class DataContainer;
}
} // namespace Data

namespace Settings
{
namespace Presets
{
class Preset;
}
} // namespace Settings

namespace AppUI
{
namespace Setup
{
class DialogPreview;

namespace Parts
{
namespace Prepare
{
class SetupTabPrepare;
}
namespace Image
{
class SetupTabImage;
}
namespace Font
{
class SetupTabFont;
}
namespace Matrix
{
class SetupTabMatrix;
}
namespace Reordering
{
class SetupTabReordering;
}
namespace Templates
{
class SetupTabTemplates;
}
} // namespace Parts

class DialogOptions : public QDialog
{
  Q_OBJECT

public:
  explicit DialogOptions(Data::Containers::DataContainer* dataContainer, QWidget* parent = 0);
  virtual ~DialogOptions();

private:
  Ui::DialogOptions* ui;

  Data::Containers::DataContainer* mData;

  Settings::Presets::Preset* mPreset;
  bool mPresetChanged;

  DialogPreview* mPreview;

  Parts::Prepare::SetupTabPrepare* mSetupPrepare;
  Parts::Matrix::SetupTabMatrix* mSetupMatrix;
  Parts::Reordering::SetupTabReordering* mSetupReordering;
  Parts::Image::SetupTabImage* mSetupImage;
  Parts::Font::SetupTabFont* mSetupFont;
  Parts::Templates::SetupTabTemplates* mSetupTemplates;

  void fillPresetsList(const QString& defaultName = QString());
  void presetLoad(const QString& name);
  void presetSaveAs(const QString& name);
  void presetRemove(const QString& name);
  void createPresetsDefault();
  bool checkOverwrite(const QString& originalName, QString* resultName) const;
private slots:
  void previewUpdate();
  void on_pushButtonPreview_clicked();
  void on_pushButtonPresetSaveAs_clicked();
  void on_pushButtonPresetRemove_clicked();
  void on_pushButtonPresetImport_clicked();
  void on_pushButtonPresetExport_clicked();
  void on_comboBoxPresets_currentIndexChanged(int index);
  void previewClosed();
  void presetOverwiteNameChanged(const QString& value);

protected:
  void done(int result) Q_DECL_OVERRIDE;

signals:
  void presetLoaded();
};

} // namespace Setup
} // namespace AppUI

#endif // DIALOGOPTIONS_H
