/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef DIALOGCANVASRESIZE_H
#define DIALOGCANVASRESIZE_H

#include <QDialog>

template <class Key, class Value> class QMap;

namespace Ui
{
class DialogCanvasResize;
}

#include <QItemSelection>

namespace Data
{
namespace Containers
{
class DataContainer;
}
namespace Models
{
class ImagesModel;
class ImagesScaledProxy;
class ImagesFilterProxy;
class ColumnsReorderProxy;
class TransposeProxy;
class CanvasModProxy;
} // namespace Models
class CanvasModInfo;
} // namespace Data

namespace AppUI
{
namespace CommonDialogs
{

class DialogCanvasResize : public QDialog
{
  Q_OBJECT

public:
  explicit DialogCanvasResize(Data::Containers::DataContainer* container, const QStringList& keys,
                              QWidget* parent = nullptr);
  virtual ~DialogCanvasResize() Q_DECL_OVERRIDE;

  const QMap<QString, Data::CanvasModInfo*>* resizeInfo() const;

protected:
  void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
  Ui::DialogCanvasResize* ui;

  Data::Containers::DataContainer* mContainer;
  Data::Models::ImagesModel* mModel;
  Data::Models::ImagesScaledProxy* mScaledProxy;
  Data::Models::ImagesFilterProxy* mFilter;
  Data::Models::ColumnsReorderProxy* mReorderProxy;
  Data::Models::TransposeProxy* mTranspose;
  Data::Models::CanvasModProxy* mCanvasMod;

  const QStringList& mKeys;
  QMap<QString, Data::CanvasModInfo*>* mCanvasMods;

  void optimizeHeight();
  void optimizeWidth();

private slots:
  void spinBox_valueChanged(int value);
  void on_pushButtonReset_clicked();
  void on_pushButtonOptimizeHeight_clicked();
  void on_pushButtonOptimizeWidth_clicked();
  void resizeToContents();
  void on_scaleChanged(int value);
};

} // namespace CommonDialogs
} // namespace AppUI

#endif // DIALOGCANVASRESIZE_H
