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

#ifndef SETUPTABMATRIX_H
#define SETUPTABMATRIX_H

#include <QWidget>

namespace Ui
{
class SetupTabMatrix;
}

namespace Settings
{
namespace Presets
{
class Preset;
}
} // namespace Settings

#include <QMenu>

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Matrix
{
class MatrixPreviewModel;
class MatrixItemDelegate;

class SetupTabMatrix : public QWidget
{
  Q_OBJECT

public:
  explicit SetupTabMatrix(Settings::Presets::Preset* preset, QWidget* parent = 0);
  virtual ~SetupTabMatrix();

public slots:
  void matrixChanged();

private:
  Ui::SetupTabMatrix* ui;
  Settings::Presets::Preset* mPreset;
  MatrixPreviewModel* mMatrixModel;
  MatrixItemDelegate* mMatrixItemDelegate;
  QMenu* mMenu;

  void updateMaskByBlockSize();

private slots:
  void on_tableViewOperations_customContextMenuRequested(const QPoint& point);
  void on_presetChanged(const QString& groupName);
  void operationAdd();
  void operationShift();
  void operationRemove();
  void maskReset();
};

} // namespace Matrix
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // SETUPTABMATRIX_H
