/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#include "toolpen.h"
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QColorDialog>
#include <appsettings.h>
#include "bitmaphelper.h"
#include "iimageeditorparams.h"

namespace ImageEditor
{

ToolPen::ToolPen(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
  this->mParameters = parameters;
  this->mIcon = new QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_pen"), 24)));

  this->mActions = new QList<QAction *>();
  this->mWidgets = new QList<QWidget *>();

  this->mSize = 1;

  this->loadSettings();
  this->initializeWidgets();
}

ToolPen::~ToolPen()
{
  this->saveSettings();
  delete this->mIcon;
  qDeleteAll(*this->mActions);
  qDeleteAll(*this->mWidgets);
  delete this->mActions;
  delete this->mWidgets;
}

const QString ToolPen::title() const
{
  return tr("Pen");
}

const QString ToolPen::tooltip() const
{
  return tr("Draw pixels");
}

const QIcon *ToolPen::icon() const
{
  return this->mIcon;
}

const QList<QAction *> *ToolPen::actions() const
{
  return this->mActions;
}

const QList<QWidget *> *ToolPen::widgets() const
{
  return this->mWidgets;
}

bool ToolPen::processMouse(QMouseEvent *event,
                           const QImage *imageOriginal,
                           bool inRect)
{
  if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress) {
    if (event->type() == QEvent::MouseButtonPress) {
      this->mFlagChanged = false;
    }

    // get coordinates
    if (imageOriginal != NULL) {
      if (inRect) {
        // get buttons
        bool buttonLeft = (event->buttons() & Qt::LeftButton) == Qt::LeftButton;
        bool buttonRight = (event->buttons() & Qt::RightButton) == Qt::RightButton;

        // draw on pixmap
        if (buttonLeft) {
          if (!this->mFlagChanged) {
            this->mInternalImage = *imageOriginal;
          }

          this->drawPixel(event->x(), event->y(), this->mParameters->foreColor());
          this->mFlagChanged = true;
          emit this->processing(&this->mInternalImage);
        }

        if (buttonRight) {
          if (!this->mFlagChanged) {
            this->mInternalImage = *imageOriginal;
          }

          this->drawPixel(event->x(), event->y(), this->mParameters->backColor());
          this->mFlagChanged = true;
          emit this->processing(&this->mInternalImage);
        }
      }
    }

    event->accept();
  } else if (event->type() == QEvent::MouseButtonRelease) {
    emit this->completed(&this->mInternalImage, this->mFlagChanged);
  }

  return true;
}

void ToolPen::initializeWidgets()
{
  QSpinBox *spinBoxSize = new QSpinBox();
  spinBoxSize->setMinimum(1);
  spinBoxSize->setSuffix(QString("px"));
  spinBoxSize->setValue(this->mSize);
  spinBoxSize->setToolTip(tr("Size"));
  this->connect(spinBoxSize, SIGNAL(valueChanged(int)), SLOT(on_spinBoxSize_valueChanged(int)));
  this->mWidgets->append(spinBoxSize);
}

void ToolPen::loadSettings()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("pen");

  bool ok;
  int a = sett.value("size", QVariant(1)).toInt(&ok);

  if (ok) {
    this->mSize = a;
  }

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolPen::saveSettings() const
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("pen");

  sett.setValue("size", QVariant(this->mSize));

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolPen::drawPixel(int x, int y, const QColor &color)
{
  QImage image = this->mInternalImage;
  QPixmap pixmap = QPixmap::fromImage(image);
  QPainter painter(&pixmap);

  if (!this->mParameters->selectedPath().isEmpty()) {
    painter.setClipPath(this->mParameters->selectedPath());
  }

  if (this->mSize == 1) {
    painter.setPen(color);
    painter.drawPoint(x, y);
  } else {
    QRect rect = QRect(x, y, this->mSize, this->mSize);
    painter.fillRect(rect, color);
  }

  this->mInternalImage = pixmap.toImage();
}

void ToolPen::on_spinBoxSize_valueChanged(int value)
{
  this->mSize = value;
}

} // end of namespace

