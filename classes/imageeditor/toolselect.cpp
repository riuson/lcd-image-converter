/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "toolselect.h"

#include <QAction>
#include <QColor>
#include <QList>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QToolButton>
#include <QWidget>

#include <appsettings.h>

#include "bitmaphelper.h"
#include "iimageeditorparams.h"

namespace ImageEditor
{
namespace Tools
{

ToolSelect::ToolSelect(IImageEditorParams* parameters, QObject* parent) : QObject(parent)
{
  this->mParameters = parameters;
  this->mIcon = new QIcon(
      QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_select"), 24)));

  this->mActions = new QList<QAction*>();
  this->mWidgets = new QList<QWidget*>();

  this->initializeWidgets();
}

ToolSelect::~ToolSelect()
{
  delete this->mIcon;
  qDeleteAll(*this->mActions);
  qDeleteAll(*this->mWidgets);
  delete this->mActions;
  delete this->mWidgets;
}

const QString ToolSelect::title() const { return tr("Select"); }

const QString ToolSelect::tooltip() const { return tr("Select area"); }

const QIcon* ToolSelect::icon() const { return this->mIcon; }

const QList<QAction*>* ToolSelect::actions() const { return this->mActions; }

const QList<QWidget*>* ToolSelect::widgets() const { return this->mWidgets; }

const QPainterPath& ToolSelect::selectedPath() const { return this->mSelectedPath; }

bool ToolSelect::processMouse(QMouseEvent* event, const QImage* imageOriginal, bool inRect)
{
  Q_UNUSED(inRect)

  if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress) {
    if (event->type() == QEvent::MouseButtonPress) {
      this->mFlagChanged = false;
    }

    // get coordinates
    if (imageOriginal != nullptr) {
      Qt::MouseButtons buttons = event->buttons();
      int x = event->x();
      int y = event->y();

      switch (this->mToolMode) {
        case SelectionEdit:
          this->processModeEdit(buttons, x, y);
          break;

        case SelectionMove:
          this->processModeMove(buttons, x, y);
          break;
      }
    }

    event->accept();
  } else if (event->type() == QEvent::MouseButtonRelease) {
    // emit this->completed(&this->mOriginalImage, false);
    emit this->selectionChanged(this->mSelectedPath);
  }

  return true;
}

void ToolSelect::initializeWidgets()
{
  QActionGroup* group = new QActionGroup(this);

  this->mActionEditSelection = new QAction(this);
  this->mActionEditSelection->setCheckable(true);
  this->mActionEditSelection->setText(tr("Modify selection"));
  this->mActionEditSelection->setToolTip(
      tr("<b>Modify selection</b><br/>Left mouse button to add selection.<br/>Right mouse button to subtract "
         "selection.<br/>Middle mouse button to reset selection."));
  this->mActionEditSelection->setIcon(QIcon(QPixmap::fromImage(
      Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_select_edit"), 24))));
  this->connect(this->mActionEditSelection, SIGNAL(triggered()), SLOT(on_switchToSelectionEdit()));
  this->mActions->append(this->mActionEditSelection);
  group->addAction(this->mActionEditSelection);

  this->mActionMoveSelection = new QAction(this);
  this->mActionMoveSelection->setCheckable(true);
  this->mActionMoveSelection->setText(tr("Move selection"));
  this->mActionMoveSelection->setToolTip(tr("<b>Move selection</b><br/>Use left mouse button to move selection."));
  this->mActionMoveSelection->setIcon(QIcon(QPixmap::fromImage(
      Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_select_move"), 24))));
  this->connect(this->mActionMoveSelection, SIGNAL(triggered()), SLOT(on_switchToSelectionMove()));
  this->mActions->append(this->mActionMoveSelection);
  group->addAction(this->mActionMoveSelection);

  this->mActionEditSelection->setChecked(true);
  this->mToolMode = SelectionEdit;
}

void ToolSelect::modifySelection(const QRect& rect, Operation op)
{
  switch (op) {
    case ToolSelect::Append: {
      QPainterPath newPath; // = this->mSelectedPath;
      newPath.addRect(rect);
      newPath += this->mSelectedPathInternal;
      this->mSelectedPath = newPath.simplified();
      break;
    }

    case ToolSelect::Subtract: {
      QPainterPath path;
      path.addRect(rect);
      QPainterPath newPath = this->mSelectedPathInternal.subtracted(path);
      this->mSelectedPath = newPath.simplified();
      break;
    }

    case ToolSelect::Reset: {
      this->mSelectedPath = QPainterPath();
      break;
    }

    default: {
      break;
    }
  }
}

void ToolSelect::processModeEdit(Qt::MouseButtons buttons, int x, int y)
{
  Operation op = ToolSelect::None;

  if ((buttons & Qt::LeftButton) == Qt::LeftButton) {
    op = ToolSelect::Append;
  }

  if ((buttons & Qt::RightButton) == Qt::RightButton) {
    op = ToolSelect::Subtract;
  }

  if (((buttons & Qt::RightButton) == Qt::RightButton) && ((buttons & Qt::LeftButton) == Qt::LeftButton)) {
    op = ToolSelect::Reset;
  }

  if ((buttons & Qt::MiddleButton) == Qt::MiddleButton) {
    op = ToolSelect::Reset;
  }

  // draw on pixmap
  if (op != ToolSelect::None) {
    if (!this->mFlagChanged) {
      this->mStartPoint = QPoint(x, y);
      this->mSelectedPathInternal = this->mSelectedPath;
    }

    int x1 = this->mStartPoint.x();
    int y1 = this->mStartPoint.y();
    int x2 = x;
    int y2 = y;

    if (x2 < x1) {
      qSwap(x1, x2);
    }

    if (y2 < y1) {
      qSwap(y1, y2);
    }

    QRect rect;
    rect.setCoords(x1, y1, x2, y2);
    this->modifySelection(rect, op);
    this->mFlagChanged = true;
    emit this->selectionChanged(this->mSelectedPath);
  }
}

void ToolSelect::processModeMove(Qt::MouseButtons buttons, int x, int y)
{
  if (this->mSelectedPath.isEmpty()) {
    return;
  }

  if ((buttons & Qt::LeftButton) == Qt::LeftButton) {
    if (!this->mFlagChanged) {
      this->mStartPoint = QPoint(x, y);
      this->mSelectedPathInternal = this->mSelectedPath;
    }

    int x1 = this->mStartPoint.x();
    int y1 = this->mStartPoint.y();
    int x2 = x;
    int y2 = y;

    QPainterPath path = this->mSelectedPathInternal;
    QPoint offset = QPoint(x2 - x1, y2 - y1);
    this->mSelectedPath = path.translated(offset);

    this->mFlagChanged = true;
    emit this->selectionChanged(this->mSelectedPath);
  }
}

void ToolSelect::on_switchToSelectionEdit() { this->mToolMode = SelectionEdit; }

void ToolSelect::on_switchToSelectionMove() { this->mToolMode = SelectionMove; }

} // namespace Tools
} // namespace ImageEditor
