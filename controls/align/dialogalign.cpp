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

#include "dialogalign.h"
#include "ui_dialogalign.h"

#include <QItemSelectionModel>
#include <QMap>
#include <QWheelEvent>
#include "alignmodes.h"
#include "alignmodinfo.h"
#include "alignmodproxy.h"
#include "imagesmodel.h"
#include "imagesalignedproxy.h"
#include "imagesscaledproxy.h"
#include "imagesfilterproxy.h"
#include "transposeproxy.h"
#include "columnsreorderproxy.h"
#include "alignsettings.h"
#include "datacontainer.h"
#include "bitmaphelper.h"

namespace AppUI
{
namespace CommonDialogs
{

DialogAlign::DialogAlign(Data::Containers::DataContainer *container,
                         const QStringList &keys,
                         QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogAlign),
  mKeys(keys)
{
  ui->setupUi(this);

  this->mContainer = container;

  this->mAlignModInfo = new Data::AlignModInfo();

  this->mModel = new Data::Models::ImagesModel(container, this);

  this->mAlignMod = new Data::Models::AlignModProxy(this->mAlignModInfo, this);
  this->mAlignMod->setSourceModel(this->mModel);

  this->mScaledProxy = new Data::Models::ImagesScaledProxy(this);
  this->mScaledProxy->setSourceModel(this->mAlignMod);

  this->mFilter = new Data::Models::ImagesFilterProxy(this);
  this->mFilter->setSourceModel(this->mScaledProxy);
  this->mFilter->setFilter(this->mKeys);

  this->mReorderProxy = new Data::Models::ColumnsReorderProxy();
  this->mReorderProxy->setSourceModel(this->mFilter);
  this->mReorderProxy->setReorder(1, 3);

  this->mTranspose = new Data::Models::TransposeProxy(this);
  this->mTranspose->setSourceModel(this->mReorderProxy);

  this->ui->tableView->setModel(this->mTranspose);

  this->connect(this->ui->spinBoxHorizontalOffset, SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxVerticalOffset,   SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
  this->connect(this->ui->spinBoxScale,  SIGNAL(valueChanged(int)), this->mScaledProxy, SLOT(setScale(int)));
  this->connect(this->mScaledProxy, SIGNAL(scaleChanged(int)), SLOT(on_scaleChanged(int)));

  int scale = Settings::AlignSettings::scale();
  this->mScaledProxy->setScale(scale);
  this->on_scaleChanged(scale);
}

DialogAlign::~DialogAlign()
{
  Settings::AlignSettings::setScale(this->ui->spinBoxScale->value());
  delete this->mAlignModInfo;
  delete ui;
}

const Data::AlignModInfo *DialogAlign::alignInfo() const
{
  return this->mAlignModInfo;
}

void DialogAlign::wheelEvent(QWheelEvent *event)
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

void DialogAlign::spinBox_valueChanged(int value)
{
  Q_UNUSED(value);

  qint16 horizontalOffset = static_cast<qint16>(this->ui->spinBoxHorizontalOffset->value());
  qint16 verticalOffset = static_cast<qint16>(this->ui->spinBoxVerticalOffset->value());
  this->mAlignModInfo->modify(
    Data::HorizontalAlignMode::None,
    horizontalOffset,
    Data::VerticalAlignMode::None,
    verticalOffset);

  // this->resizeToContents();
}

void DialogAlign::resizeToContents()
{
  this->ui->tableView->resizeRowsToContents();
  this->ui->tableView->resizeColumnsToContents();
}

void DialogAlign::on_scaleChanged(int value)
{
  if (this->ui->spinBoxScale->value() != value) {
    this->ui->spinBoxScale->setValue(value);
  }

  this->resizeToContents();
}

} // namespace CommonDialogs
} // namespace AppUI
