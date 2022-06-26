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

#include "dialogcanvasresize.h"
#include "ui_dialogcanvasresize.h"

#include <QItemSelectionModel>
#include <QMap>
#include <QWheelEvent>
#include "canvasmodinfo.h"
#include "canvasmodproxy.h"
#include "imagesmodel.h"
#include "imagesscaledproxy.h"
#include "imagesfilterproxy.h"
#include "transposeproxy.h"
#include "columnsreorderproxy.h"
#include "resizesettings.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace AppUI
{
namespace CommonDialogs
{

DialogCanvasResize::DialogCanvasResize(Data::Containers::DataContainer *container,
                                       const QStringList &keys,
                                       QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogCanvasResize),
  mKeys(keys)
{
  ui->setupUi(this);

  this->mContainer = container;

  this->mCanvasMods = new QMap<QString, Data::CanvasModInfo *>();

  for (auto key : container->keys()) {
    this->mCanvasMods->insert(key, new Data::CanvasModInfo());
  }

  this->mModel = new Data::Models::ImagesModel(container, this);

  this->mCanvasMod = new Data::Models::CanvasModProxy(this->mCanvasMods, this);
  this->mCanvasMod->setSourceModel(this->mModel);

  this->mScaledProxy = new Data::Models::ImagesScaledProxy(this);
  this->mScaledProxy->setSourceModel(this->mCanvasMod);

  this->mFilter = new Data::Models::ImagesFilterProxy(this);
  this->mFilter->setSourceModel(this->mScaledProxy);
  this->mFilter->setFilter(this->mKeys);

  this->mReorderProxy = new Data::Models::ColumnsReorderProxy();
  this->mReorderProxy->setSourceModel(this->mFilter);
  this->mReorderProxy->setReorder(1, 3);

  this->mTranspose = new Data::Models::TransposeProxy(this);
  this->mTranspose->setSourceModel(this->mReorderProxy);

  this->ui->tableView->setModel(this->mTranspose);

  this->connect(this->ui->spinBoxLeft,   SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxTop,    SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxRight,  SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxBottom, SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxScale,  SIGNAL(valueChanged(int)), this->mScaledProxy, SLOT(setScale(int)));
  this->connect(this->mScaledProxy, SIGNAL(scaleChanged(int)), SLOT(on_scaleChanged(int)));

  int scale = Settings::ResizeSettings::scale();
  this->mScaledProxy->setScale(scale);
  this->on_scaleChanged(scale);
}

DialogCanvasResize::~DialogCanvasResize()
{
  Settings::ResizeSettings::setScale(this->ui->spinBoxScale->value());
  qDeleteAll(*this->mCanvasMods);
  this->mCanvasMods->clear();
  delete ui;
}

const QMap<QString, Data::CanvasModInfo *> *DialogCanvasResize::resizeInfo() const
{
  return this->mCanvasMods;
}

void DialogCanvasResize::wheelEvent(QWheelEvent *event)
{
  if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
    QPoint point = event->globalPosition().toPoint();
    point = this->mapFromGlobal(point);

    if (qAbs(event->angleDelta().x()) < qAbs(event->angleDelta().y())) {
      int scale = this->mScaledProxy->scale();

      if (event->angleDelta().y() > 0) {
        scale++;
      } else {
        scale--;
      }

      this->mScaledProxy->setScale(scale);
    }

    event->accept();
  }
}

void DialogCanvasResize::optimizeHeight()
{
  int top = std::numeric_limits<int>::max();
  int bottom = std::numeric_limits<int>::max();

  for (auto key : this->mKeys) {
    const QImage *original = this->mContainer->image(key);

    int l, t, r, b;
    bool hEmpty, vEmpty;
    Parsing::Conversion::BitmapHelper::findEmptyArea(original, l, t, r, b, hEmpty, vEmpty);

    if (vEmpty) {
      continue;
    }

    top = qMin(top, t);
    bottom = qMin(bottom, b);
  }

  for (auto key : this->mKeys) {
    Data::CanvasModInfo *info = this->mCanvasMods->value(key);
    info->modify(0, -top, 0, -bottom);
    info->commit();
  }
}

void DialogCanvasResize::optimizeWidth()
{
  int left = std::numeric_limits<int>::max();
  int right = std::numeric_limits<int>::max();

  for (auto key : this->mKeys) {
    const QImage *original = this->mContainer->image(key);

    int l, t, r, b;
    bool hEmpty, vEmpty;
    Parsing::Conversion::BitmapHelper::findEmptyArea(original, l, t, r, b, hEmpty, vEmpty);

    if (hEmpty) {
      continue;
    }

    left = l;
    right = r;

    Data::CanvasModInfo *info = this->mCanvasMods->value(key);
    info->modify(-left, 0, -right, 0);
    info->commit();
  }
}

void DialogCanvasResize::spinBox_valueChanged(int value)
{
  Q_UNUSED(value);

  qint16 left = static_cast<qint16>(this->ui->spinBoxLeft->value());
  qint16 top = static_cast<qint16>(this->ui->spinBoxTop->value());
  qint16 right = static_cast<qint16>(this->ui->spinBoxRight->value());
  qint16 bottom = static_cast<qint16>(this->ui->spinBoxBottom->value());

  for (auto key : this->mKeys) {
    Data::CanvasModInfo *info = this->mCanvasMods->value(key);
    info->modify(left, top, right, bottom);
  }

  this->resizeToContents();
}

void DialogCanvasResize::on_pushButtonReset_clicked()
{
  this->ui->spinBoxLeft->setValue(0);
  this->ui->spinBoxTop->setValue(0);
  this->ui->spinBoxRight->setValue(0);
  this->ui->spinBoxBottom->setValue(0);

  for (auto key : this->mKeys) {
    Data::CanvasModInfo *info = this->mCanvasMods->value(key);
    info->reset();
  }

  this->resizeToContents();
}

void DialogCanvasResize::on_pushButtonOptimizeHeight_clicked()
{
  this->optimizeHeight();
  this->resizeToContents();
}

void DialogCanvasResize::on_pushButtonOptimizeWidth_clicked()
{
  this->optimizeWidth();
  this->resizeToContents();
}

void DialogCanvasResize::resizeToContents()
{
  this->ui->tableView->resizeRowsToContents();
  this->ui->tableView->resizeColumnsToContents();
}

void DialogCanvasResize::on_scaleChanged(int value)
{
  if (this->ui->spinBoxScale->value() != value) {
    this->ui->spinBoxScale->setValue(value);
  }

  this->resizeToContents();
}

} // namespace CommonDialogs
} // namespace AppUI
