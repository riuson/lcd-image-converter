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

#ifndef TOOLSELECT_H
#define TOOLSELECT_H

#include <QAction>
#include <QColor>
#include <QIcon>
#include <QImage>
#include <QObject>
#include <QPainterPath>
#include <QPoint>

#include "iimageeditortool.h"
#include "iimageselection.h"

namespace ImageEditor
{
class IImageEditorParams;

namespace Tools
{

class ToolSelect : public QObject, public IImageEditorTool, public IImageSelection
{
  Q_OBJECT
  Q_INTERFACES(ImageEditor::Tools::IImageEditorTool)
  Q_INTERFACES(ImageEditor::IImageSelection)

public:
  explicit ToolSelect(IImageEditorParams* parameters, QObject* parent = 0);
  virtual ~ToolSelect();

  const QString title() const Q_DECL_OVERRIDE;
  const QString tooltip() const Q_DECL_OVERRIDE;
  const QIcon* icon() const Q_DECL_OVERRIDE;
  const QList<QAction*>* actions() const Q_DECL_OVERRIDE;
  const QList<QWidget*>* widgets() const Q_DECL_OVERRIDE;

  const QPainterPath& selectedPath() const Q_DECL_OVERRIDE;

public slots:
  bool processMouse(QMouseEvent* event, const QImage* imageOriginal, bool inRect) Q_DECL_OVERRIDE;

signals:
  void started(const QImage* value) Q_DECL_OVERRIDE;
  void processing(const QImage* value) Q_DECL_OVERRIDE;
  void completed(const QImage* value, bool changed) Q_DECL_OVERRIDE;
  void selectionChanged(const QPainterPath& value) Q_DECL_OVERRIDE;

private:
  enum Operation { None, Append, Subtract, Reset };

  enum Mode { SelectionEdit, SelectionMove };

  IImageEditorParams* mParameters;
  QIcon* mIcon;
  QList<QAction*>* mActions;
  QList<QWidget*>* mWidgets;
  QAction* mActionEditSelection;
  QAction* mActionMoveSelection;

  bool mFlagChanged;
  QPoint mStartPoint;
  QPainterPath mSelectedPath;
  Mode mToolMode;
  QPainterPath mSelectedPathInternal;

  void initializeWidgets();
  void modifySelection(const QRect& rect, Operation op);
  void processModeEdit(Qt::MouseButtons buttons, int x, int y);
  void processModeMove(Qt::MouseButtons buttons, int x, int y);

private slots:
  void on_switchToSelectionEdit();
  void on_switchToSelectionMove();
};

} // namespace Tools
} // namespace ImageEditor

#endif // TOOLSELECT_H
