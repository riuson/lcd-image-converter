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

#include "toolsmanager.h"

#include <QList>
#include <QAction>
#include "iimageeditortool.h"
#include "toolselect.h"
#include "toolpen.h"
#include "toolline.h"
#include "toolrect.h"
#include "toolzoom.h"
#include "toolfill.h"
#include "toolcolor.h"
#include "toolmove.h"

namespace ImageEditor
{
namespace Tools
{

ToolsManager::ToolsManager(QObject *parent) : QObject(parent)
{
  this->mParentWidget = qobject_cast<QWidget *>(parent);
  this->mTools = new QList<IImageEditorTool *> ();
  this->mToolsActions = new QList<QAction *> ();

  this->initializeTools();
  this->initializeActions();
}

ToolsManager::~ToolsManager()
{
  qDeleteAll(*this->mToolsActions);
  delete this->mToolsActions;

  qDeleteAll(*this->mTools);
  delete this->mTools;
}

const QList<IImageEditorTool *> *ToolsManager::tools() const
{
  return this->mTools;
}

const QList<QAction *> *ToolsManager::toolsActions() const
{
  return this->mToolsActions;
}

int ToolsManager::scale() const
{
  return this->mZoomer->scale();
}

const QColor ToolsManager::foreColor() const
{
  return this->mColors->foreColor();
}

const QColor ToolsManager::backColor() const
{
  return this->mColors->backColor();
}

QWidget *ToolsManager::parentWidget() const
{
  return this->mParentWidget;
}

const QPainterPath &ToolsManager::selectedPath() const
{
  return this->mSelectionTool->selectedPath();
}

void ToolsManager::setScale(int value)
{
  this->mZoomer->setScale(value);
}

void ToolsManager::initializeTools()
{
  this->mSelectionTool = new ToolSelect(this, this);
  this->connect(this->mSelectionTool, SIGNAL(selectionChanged(QPainterPath)), SIGNAL(selectionChanged(QPainterPath)));
  this->mTools->append(this->mSelectionTool);

  this->mTools->append(new ToolPen(this, this));
  this->mTools->append(new ToolLine(this, this));
  this->mTools->append(new ToolRect(this, this));
  this->mTools->append(new ToolFill(this, this));
  this->mTools->append(new ToolMove(this, this));

  this->mColors = new ToolColor(this, this);
  this->mTools->append(this->mColors);

  this->mZoomer = new ToolZoom(this, this);
  this->connect(this->mZoomer, SIGNAL(scaleChanged(int)), SIGNAL(scaleChanged(int)));
  this->mTools->append(this->mZoomer);
}

void ToolsManager::initializeActions()
{
  for (int i = 0; i < this->mTools->length(); i++) {
    IImageEditorTool *tool = this->mTools->at(i);
    QAction *toolAction = new QAction(*tool->icon(), tool->title(), this);
    toolAction->setToolTip(tool->tooltip());
    toolAction->setData(QVariant(i)); // index of the tool
    toolAction->setCheckable(true);
    this->connect(toolAction, SIGNAL(triggered()), SLOT(on_toolAction_triggered()));
    this->mToolsActions->append(toolAction);
  }
}

void ToolsManager::selectTool(IImageEditorTool *tool)
{
  int toolIndex = -1;

  for (int i = 0; i < this->mTools->length(); i++) {
    if (this->mTools->at(i) == tool) {
      this->mToolsActions->at(i)->setChecked(true);
      this->mSelectedTool = tool;
      toolIndex = i;
    } else {
      this->mToolsActions->at(i)->setChecked(false);
    }
  }

  emit this->toolChanged(toolIndex);
}

void ToolsManager::on_toolAction_triggered()
{
  QAction *action = qobject_cast<QAction *>(sender());

  if (action != nullptr) {
    bool ok;
    int toolIndex = action->data().toInt(&ok);

    if (ok) {
      IImageEditorTool *tool = this->mTools->at(toolIndex);
      this->selectTool(tool);
    }
  }
}

} // namespace Tools
} // namespace ImageEditor

