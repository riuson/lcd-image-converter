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
#include "imagesmodel.h"
#include "imagesscaledproxy.h"
#include "imagesresizedproxy.h"
#include "imagesfilterproxy.h"
#include "transposeproxy.h"
#include "columnsreorderproxy.h"
#include "resizesettings.h"

namespace AppUI
{
namespace CommonDialogs
{

DialogCanvasResize::DialogCanvasResize(Data::Containers::DataContainer *container, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogCanvasResize)
{
  ui->setupUi(this);

  this->mContainer = container;
  this->mCanvasMods = new QMap<QString, Data::CanvasModInfo *>();

  this->mModel = new Data::Models::ImagesModel(container, this);

  this->mResizedProxy = new Data::Models::ImagesResizedProxy(this);
  this->mResizedProxy->setSourceModel(this->mModel);

  this->mScaledProxy = new Data::Models::ImagesScaledProxy(this);
  this->mScaledProxy->setSourceModel(this->mResizedProxy);

  this->mFilter = new Data::Models::ImagesFilterProxy(this);
  this->mFilter->setSourceModel(this->mScaledProxy);

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

  this->mLeft = 0;
  this->mTop = 0;
  this->mRight = 0;
  this->mBottom = 0;

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

void DialogCanvasResize::selectKeys(const QStringList &keys)
{
  this->mFilter->setFilter(keys);

  this->resizeToContents();
}

void DialogCanvasResize::resizeInfo(int *left, int *top, int *right, int *bottom) const
{
  *left = this->mLeft;
  *top = this->mTop;
  *right = this->mRight;
  *bottom = this->mBottom;
}

void DialogCanvasResize::setResizeInfo(int left, int top, int right, int bottom)
{
  this->mLeft = left;
  this->mTop = top;
  this->mRight = right;
  this->mBottom = bottom;

  this->ui->spinBoxLeft->setValue(left);
  this->ui->spinBoxTop->setValue(top);
  this->ui->spinBoxRight->setValue(right);
  this->ui->spinBoxBottom->setValue(bottom);
}

void DialogCanvasResize::wheelEvent(QWheelEvent *event)
{
  if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) {
    QPoint point = event->globalPos();
    point = this->mapFromGlobal(point);

    if (event->orientation() == Qt::Vertical) {
      int scale = this->mScaledProxy->scale();

      if (event->delta() > 0) {
        scale++;
      } else {
        scale--;
      }

      this->mScaledProxy->setScale(scale);
    }

    event->accept();
  }
}

void DialogCanvasResize::spinBox_valueChanged(int value)
{
  Q_UNUSED(value);

  this->mLeft = this->ui->spinBoxLeft->value();
  this->mTop = this->ui->spinBoxTop->value();
  this->mRight = this->ui->spinBoxRight->value();
  this->mBottom = this->ui->spinBoxBottom->value();

  this->mResizedProxy->setCrop(
    this->mLeft,
    this->mTop,
    this->mRight,
    this->mBottom);

  this->resizeToContents();
}

void DialogCanvasResize::on_pushButtonReset_clicked()
{
  this->ui->spinBoxLeft->setValue(0);
  this->ui->spinBoxTop->setValue(0);
  this->ui->spinBoxRight->setValue(0);
  this->ui->spinBoxBottom->setValue(0);
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
