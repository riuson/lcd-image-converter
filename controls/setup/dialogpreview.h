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

#ifndef DIALOGPREVIEW_H
#define DIALOGPREVIEW_H

#include <QDialog>

namespace Ui
{
class DialogPreview;
}
namespace Settings
{
namespace Presets
{
class Preset;
}
}
namespace Data
{
namespace Containers
{
class DataContainer;
}
}

class DialogPreview : public QDialog
{
  Q_OBJECT

public:
  explicit DialogPreview(Data::Containers::DataContainer *dataContainer, Settings::Presets::Preset *matrix, QWidget *parent = 0);
  virtual ~DialogPreview();
  void updatePreview();

private:
  Ui::DialogPreview *ui;

  Data::Containers::DataContainer *mData;
  QImage mImageOriginal;
  QImage mImageProcessed;
  Settings::Presets::Preset *mPreset;
  int mScale;

protected:
  void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
  void setScale(int value);
  void updatePreviewScaled(const QImage *image, int scale);

private slots:
  void on_comboBoxSampleKey_currentIndexChanged();
  void on_spinBoxScale_valueChanged(int value);
};

#endif // DIALOGPREVIEW_H
