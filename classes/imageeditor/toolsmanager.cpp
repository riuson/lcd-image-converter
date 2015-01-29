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
#include "iimageeditortool.h"
#include "toolpen.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolsManager::ToolsManager(QObject *parent) : QObject(parent)
{
    this->mTools = new QList<IImageEditorTool *> ();

    this->InitializeTools();
}
//-----------------------------------------------------------------------------
ToolsManager::~ToolsManager()
{
    qDeleteAll(*this->mTools);
    delete this->mTools;
}
//-----------------------------------------------------------------------------
const QList<IImageEditorTool *> *ToolsManager::tools() const
{
    return this->mTools;
}
//-----------------------------------------------------------------------------
void ToolsManager::InitializeTools()
{
    IImageEditorTool *tool = new ToolPen(this);
    this->mTools->append(tool);
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
