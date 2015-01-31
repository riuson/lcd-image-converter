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
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
Editor::Editor(QObject *parent) : QObject(parent)
{
    this->mTools = new ToolsManager(this);
    this->mWidget = new WindowEditor();
    this->mWidget->setTools(this->mTools);
    this->connect(this->mWidget, SIGNAL(imageChanged()), SLOT(on_imageChanged()));
    this->connect(this->mWidget, SIGNAL(mouseMove(const QPoint *)), SLOT(on_mouseMove(const QPoint*)));
    this->connect(this->mWidget, SIGNAL(scaleSchanged(int)), SLOT(on_scaleSchanged(int)));
    this->mForeColor = new QColor();
    this->mBackColor = new QColor();

    this->connect(this->mTools, SIGNAL(zoomChanged(int)), SLOT(on_scaleSchanged(int)));
}
//-----------------------------------------------------------------------------
Editor::~Editor()
{
    delete this->mForeColor;
    delete this->mBackColor;

    // delete widgets of tools before editor widget
    delete this->mTools;
    delete this->mWidget;
}
//-----------------------------------------------------------------------------
QWidget *Editor::widget() const
{
    return this->mWidget;
}
//-----------------------------------------------------------------------------
const QImage *Editor::image() const
{
    return this->mWidget->image();
}
//-----------------------------------------------------------------------------
void Editor::setImage(const QImage *_value)
{
    this->mWidget->setImage(_value);
}
//-----------------------------------------------------------------------------
const QColor *Editor::foreColor() const
{
    return this->mForeColor;
}
//-----------------------------------------------------------------------------
const QColor *Editor::backColor() const
{
    return this->mBackColor;
}
//-----------------------------------------------------------------------------
int Editor::scale() const
{
    return this->mWidget->scale();
}
//-----------------------------------------------------------------------------
void Editor::on_imageChanged()
{
    emit this->imageChanged(this->image());
}
//-----------------------------------------------------------------------------
void Editor::on_mouseMove(const QPoint *point)
{
    emit this->mouseMoved(point);
}
//-----------------------------------------------------------------------------
void Editor::on_scaleSchanged(int scale)
{
    emit this->scaleChanged(scale);
    this->mWidget->setScale(scale);
    this->mTools->setZoom(scale);
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
