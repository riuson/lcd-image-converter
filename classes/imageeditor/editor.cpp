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

#include "editor.h"
#include "windoweditor.h"

#include <QColor>
#include "toolsmanager.h"

namespace ImageEditor
{

Editor::Editor(QObject *parent) : QObject(parent)
{
  this->mWidget = new AppUI::Images::WindowEditor();
  this->connect(this->mWidget, SIGNAL(imageChanged()), SLOT(on_imageChanged()));
  this->connect(this->mWidget, SIGNAL(mouseMove(const QPoint *)), SLOT(on_mouseMove(const QPoint *)));
  this->connect(this->mWidget, SIGNAL(scaleChanged(int)), SLOT(on_scaleChanged(int)));

  this->connect(this->mWidget, SIGNAL(scaleChanged(int)), SLOT(on_scaleChanged(int)));
}

Editor::~Editor()
{
  // delete widgets of tools before editor widget
  delete this->mWidget;
}

QWidget *Editor::widget() const
{
  return this->mWidget;
}

const QImage *Editor::image() const
{
  return this->mWidget->image();
}

void Editor::setImage(const QImage *value)
{
  this->mWidget->setImage(value);
}

int Editor::scale() const
{
  return this->mWidget->scale();
}

void Editor::on_imageChanged()
{
  emit this->imageChanged(this->image());
}

void Editor::on_mouseMove(const QPoint *point)
{
  emit this->mouseMoved(point);
}

void Editor::on_scaleChanged(int value)
{
  if (value > 0) {
    emit this->scaleChanged(value);
  }
}

} // namespace ImageEditor

