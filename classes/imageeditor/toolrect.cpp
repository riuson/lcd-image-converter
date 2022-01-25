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

#include "toolrect.h"
#include <QPainter>
#include <QPainterPath>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QColorDialog>
#include <QPainterPath>
#include <appsettings.h>
#include "bitmaphelper.h"
#include "iimageeditorparams.h"

namespace ImageEditor
{
namespace Tools
{

ToolRect::ToolRect(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
  this->mParameters = parameters;
  this->mIcon = new QIcon(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_rect"), 24)));

  this->mActions = new QList<QAction *>();
  this->mWidgets = new QList<QWidget *>();

  this->mSize = 1;

  this->loadSettings();
  this->initializeWidgets();
}

ToolRect::~ToolRect()
{
  this->saveSettings();
  delete this->mIcon;
  qDeleteAll(*this->mActions);
  qDeleteAll(*this->mWidgets);
  delete this->mActions;
  delete this->mWidgets;
}

const QString ToolRect::title() const
{
  return tr("Rectangle");
}

const QString ToolRect::tooltip() const
{
  return tr("<b>Draw rectangle</b><br/>Left mouse button: outline - forecolor, filling - backcolor.<br/>Right mouse button: outline - backcolor, filling - forecolor.");
}

const QIcon *ToolRect::icon() const
{
  return this->mIcon;
}

const QList<QAction *> *ToolRect::actions() const
{
  return this->mActions;
}

const QList<QWidget *> *ToolRect::widgets() const
{
  return this->mWidgets;
}

bool ToolRect::processMouse(QMouseEvent *event,
                            const QImage *imageOriginal,
                            bool inRect)
{
  Q_UNUSED(inRect)

  if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress) {
    if (event->type() == QEvent::MouseButtonPress) {
      this->mFlagChanged = false;
    }

    // get coordinates
    if (imageOriginal != nullptr) {
      // get buttons
      bool buttonLeft = (event->buttons() & Qt::LeftButton) == Qt::LeftButton;
      bool buttonRight = (event->buttons() & Qt::RightButton) == Qt::RightButton;

      // draw on pixmap
      if (buttonLeft) {
        if (!this->mFlagChanged) {
          this->mOriginalImage = *imageOriginal;
          this->mStartPoint = event->pos();
        }

        QRect rect;
        rect.setCoords(this->mStartPoint.x(), this->mStartPoint.y(), event->x(), event->y());
        this->drawRect(rect, this->mOutlineMode, this->mSize, false);
        this->mFlagChanged = true;
        emit this->processing(&this->mInternalImage);
      }

      if (buttonRight) {
        if (!this->mFlagChanged) {
          this->mOriginalImage = *imageOriginal;
          this->mStartPoint = event->pos();
        }

        QRect rect;
        rect.setCoords(this->mStartPoint.x(), this->mStartPoint.y(), event->x(), event->y());
        this->drawRect(rect, this->mOutlineMode, this->mSize, true);
        this->mFlagChanged = true;
        emit this->processing(&this->mInternalImage);
      }
    }

    event->accept();
  } else if (event->type() == QEvent::MouseButtonRelease) {
    emit this->completed(&this->mInternalImage, this->mFlagChanged);
  }

  return true;
}

void ToolRect::initializeWidgets()
{
  QSpinBox *spinBoxSize = new QSpinBox();
  spinBoxSize->setMinimum(1);
  spinBoxSize->setSuffix(QString("px"));
  spinBoxSize->setValue(this->mSize);
  spinBoxSize->setToolTip(tr("Outline Width"));
  this->connect(spinBoxSize, SIGNAL(valueChanged(int)), SLOT(on_spinBoxSize_valueChanged(int)));
  this->mWidgets->append(spinBoxSize);

  this->mActionRectOutline = new QAction(this);
  this->mActionRectOutline->setCheckable(true);
  this->mActionRectOutline->setChecked(this->mOutlineMode == Outline);
  this->mActionRectOutline->setText(tr("Outline"));
  this->mActionRectOutline->setToolTip(tr("Outline only"));
  this->mActionRectOutline->setData(QVariant((int)Outline));
  this->connect(this->mActionRectOutline, SIGNAL(triggered()), SLOT(on_buttonRertFilledOutline_triggered()));
  this->mActionRectOutline->setIcon(QIcon(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_rect_outline"), 24))));
  this->mActions->append(this->mActionRectOutline);

  this->mActionRectFilledOutline = new QAction(this);
  this->mActionRectFilledOutline->setCheckable(true);
  this->mActionRectFilledOutline->setChecked(this->mOutlineMode == FilledOutline);
  this->mActionRectFilledOutline->setText(tr("Filled Outline"));
  this->mActionRectFilledOutline->setToolTip(tr("Filled with Outline"));
  this->mActionRectFilledOutline->setData(QVariant((int)FilledOutline));
  this->connect(this->mActionRectFilledOutline, SIGNAL(triggered()), SLOT(on_buttonRertFilledOutline_triggered()));
  this->mActionRectFilledOutline->setIcon(QIcon(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_rect_filled_outline"), 24))));
  this->mActions->append(this->mActionRectFilledOutline);

  this->mActionRectFilled = new QAction(this);
  this->mActionRectFilled->setCheckable(true);
  this->mActionRectFilled->setChecked(this->mOutlineMode == Filled);
  this->mActionRectFilled->setText(tr("Filled"));
  this->mActionRectFilled->setToolTip(tr("Filled"));
  this->mActionRectFilled->setData(QVariant((int)Filled));
  this->connect(this->mActionRectFilled, SIGNAL(triggered()), SLOT(on_buttonRertFilledOutline_triggered()));
  this->mActionRectFilled->setIcon(QIcon(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_rect_filled"), 24))));
  this->mActions->append(this->mActionRectFilled);
}

void ToolRect::loadSettings()
{
  Settings::AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("rect");

  bool ok;
  int a = sett.value("size", QVariant(1)).toInt(&ok);

  if (ok) {
    this->mSize = a;
  }

  a = sett.value("outlineMode", QVariant((int)Outline)).toInt(&ok);

  if (ok) {
    this->mOutlineMode = (OutlineMode)a;
  }

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolRect::saveSettings() const
{
  Settings::AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("rect");

  sett.setValue("size", QVariant(this->mSize));
  sett.setValue("outlineMode", QVariant((int)this->mOutlineMode));

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolRect::drawRect(const QRect &rect, OutlineMode mode, int borderWidth, bool inverted)
{
  QImage image = this->mOriginalImage;
  QPixmap pixmap = QPixmap::fromImage(image);
  QPainter painter(&pixmap);

  QColor fc = this->mParameters->foreColor();
  QColor bc = this->mParameters->backColor();

  if (!this->mParameters->selectedPath().isEmpty()) {
    painter.setClipPath(this->mParameters->selectedPath());
  }

  if (inverted) {
    bc = this->mParameters->foreColor();
    fc = this->mParameters->backColor();
  }

  if (mode == Filled || mode == FilledOutline) {
    painter.setRenderHint(QPainter::Antialiasing, false);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, false);
    painter.fillRect(rect, bc);
  }

  if (mode == Outline || mode == FilledOutline) {
    QPen pen(fc, borderWidth);
    painter.setRenderHint(QPainter::Antialiasing, false);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, false);
    painter.setPen(pen);
    painter.drawRect(rect);
  }

  this->mInternalImage = pixmap.toImage();
}

void ToolRect::on_spinBoxSize_valueChanged(int value)
{
  this->mSize = value;
}

void ToolRect::on_buttonRertFilledOutline_triggered()
{
  QAction *action = qobject_cast<QAction *>(sender());

  if (action != nullptr) {
    bool ok;
    int a = action->data().toInt(&ok);

    if (ok) {
      OutlineMode mode = (OutlineMode)a;
      this->mOutlineMode = mode;

      this->mActionRectOutline->setChecked(this->mOutlineMode == Outline);
      this->mActionRectFilledOutline->setChecked(this->mOutlineMode == FilledOutline);
      this->mActionRectFilled->setChecked(this->mOutlineMode == Filled);
    }
  }
}

} // namespace Tools
} // namespace ImageEditor
