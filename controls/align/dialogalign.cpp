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
  this->mReorderProxy->setReorder(1, 2);

  this->mTranspose = new Data::Models::TransposeProxy(this);
  this->mTranspose->setSourceModel(this->mReorderProxy);

  this->ui->tableView->setModel(this->mTranspose);

  this->ui->comboBoxHorizontalMode->addItem(tr("None"),         QVariant((int)Data::HorizontalAlignMode::None));
  this->ui->comboBoxHorizontalMode->addItem(tr("Left"),         QVariant((int)Data::HorizontalAlignMode::Left));
  this->ui->comboBoxHorizontalMode->addItem(tr("Center Left"),  QVariant((int)Data::HorizontalAlignMode::CenterLeft));
  this->ui->comboBoxHorizontalMode->addItem(tr("Center Right"), QVariant((int)Data::HorizontalAlignMode::CenterRight));
  this->ui->comboBoxHorizontalMode->addItem(tr("Right"),        QVariant((int)Data::HorizontalAlignMode::Right));

  this->ui->comboBoxVerticalMode->addItem(tr("None"),          QVariant((int)Data::VerticalAlignMode::None));
  this->ui->comboBoxVerticalMode->addItem(tr("Top"),           QVariant((int)Data::VerticalAlignMode::Top));
  this->ui->comboBoxVerticalMode->addItem(tr("Center Top"),    QVariant((int)Data::VerticalAlignMode::CenterTop));
  this->ui->comboBoxVerticalMode->addItem(tr("Center Bottom"), QVariant((int)Data::VerticalAlignMode::CenterBottom));
  this->ui->comboBoxVerticalMode->addItem(tr("Bottom"),        QVariant((int)Data::VerticalAlignMode::Bottom));

  this->connect(this->ui->comboBoxHorizontalMode,  SIGNAL(currentIndexChanged(int)), SLOT(operationValuesChanged(int)));
  this->connect(this->ui->comboBoxVerticalMode,    SIGNAL(currentIndexChanged(int)), SLOT(operationValuesChanged(int)));
  this->connect(this->ui->spinBoxHorizontalOffset, SIGNAL(valueChanged(int)), SLOT(operationValuesChanged(int)));
  this->connect(this->ui->spinBoxVerticalOffset,   SIGNAL(valueChanged(int)), SLOT(operationValuesChanged(int)));
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

void DialogAlign::operationValuesChanged(int value)
{
  Q_UNUSED(value);

  QVariant data = this->ui->comboBoxHorizontalMode->itemData(this->ui->comboBoxHorizontalMode->currentIndex());
  bool isHorizontalOffsetOk;
  int horizontalMode = data.toInt(&isHorizontalOffsetOk);

  data = this->ui->comboBoxVerticalMode->itemData(this->ui->comboBoxVerticalMode->currentIndex());
  bool isVerticalOffsetOk;
  int verticalMode = data.toInt(&isVerticalOffsetOk);

  if (isHorizontalOffsetOk && isVerticalOffsetOk) {
    int horizontalOffset = this->ui->spinBoxHorizontalOffset->value();
    int verticalOffset = this->ui->spinBoxVerticalOffset->value();
    this->mAlignModInfo->modify(
      (Data::HorizontalAlignMode)horizontalMode,
      horizontalOffset,
      (Data::VerticalAlignMode)verticalMode,
      verticalOffset);

    // TODO: Workaround for TableView update.
    auto focused = this->focusWidget();
    this->ui->spinBoxHorizontalOffset->setFocus();
    this->ui->tableView->setFocus();
    focused->setFocus();

    // this->resizeToContents();
  }
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
