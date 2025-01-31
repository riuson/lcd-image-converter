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

#include "toolline.h"

#include <QAction>
#include <QColor>
#include <QColorDialog>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QSpinBox>
#include <QToolButton>
#include <QWidget>

#include "appsettings.h"
#include "bitmaphelper.h"
#include "iimageeditorparams.h"

namespace ImageEditor
{
namespace Tools
{

ToolLine::ToolLine(IImageEditorParams* parameters, QObject* parent) : QObject(parent)
{
  this->mParameters = parameters;
  this->mIcon = new QIcon(
      QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_line"), 24)));

  this->mActions = new QList<QAction*>();
  this->mWidgets = new QList<QWidget*>();

  this->mSize = 1;

  this->loadSettings();
  this->initializeWidgets();
}

ToolLine::~ToolLine()
{
  this->saveSettings();
  delete this->mIcon;
  qDeleteAll(*this->mActions);
  qDeleteAll(*this->mWidgets);
  delete this->mActions;
  delete this->mWidgets;
}

const QString ToolLine::title() const { return tr("Line"); }

const QString ToolLine::tooltip() const
{
  return tr(
      "<b>Draw line</b><br/>Use left mouse button to draw forecolor.<br/>Use right mouse button to draw backcolor.");
}

const QIcon* ToolLine::icon() const { return this->mIcon; }

const QList<QAction*>* ToolLine::actions() const { return this->mActions; }

const QList<QWidget*>* ToolLine::widgets() const { return this->mWidgets; }

bool ToolLine::processMouse(QMouseEvent* event, const QImage* imageOriginal, bool inRect)
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
        this->drawLine(rect, this->mSize, false);
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
        this->drawLine(rect, this->mSize, true);
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

void ToolLine::initializeWidgets()
{
  QSpinBox* spinBoxSize = new QSpinBox();
  spinBoxSize->setMinimum(1);
  spinBoxSize->setSuffix(QString("px"));
  spinBoxSize->setValue(this->mSize);
  spinBoxSize->setToolTip(tr("Line Width"));
  this->connect(spinBoxSize, SIGNAL(valueChanged(int)), SLOT(on_spinBoxSize_valueChanged(int)));
  this->mWidgets->append(spinBoxSize);
}

void ToolLine::loadSettings()
{
  Settings::AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("line");

  bool ok;
  int a = sett.value("size", QVariant(1)).toInt(&ok);

  if (ok) {
    this->mSize = a;
  }

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolLine::saveSettings() const
{
  Settings::AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("tools");
  sett.beginGroup("line");

  sett.setValue("size", QVariant(this->mSize));

  sett.endGroup();
  sett.endGroup();
  sett.endGroup();
}

void ToolLine::drawLine(const QRect& rect, int borderWidth, bool inverted)
{
  QImage image = this->mOriginalImage;
  QPixmap pixmap = QPixmap::fromImage(image);
  QPainter painter(&pixmap);

  if (!this->mParameters->selectedPath().isEmpty()) {
    painter.setClipPath(this->mParameters->selectedPath());
  }

  QColor fc = this->mParameters->foreColor();
  QColor bc = this->mParameters->backColor();

  if (inverted) {
    bc = this->mParameters->foreColor();
    fc = this->mParameters->backColor();
  }

  QPen pen(fc, borderWidth);
  painter.setRenderHint(QPainter::Antialiasing, false);
  // painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing, false);
  painter.setPen(pen);
  painter.drawLine(rect.left(), rect.top(), rect.right(), rect.bottom());

  this->mInternalImage = pixmap.toImage();
}

void ToolLine::on_spinBoxSize_valueChanged(int value) { this->mSize = value; }

} // namespace Tools
} // namespace ImageEditor
