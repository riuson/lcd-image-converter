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

#ifndef TOOLSMANAGER_H
#define TOOLSMANAGER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
template <class T1> class QList;
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
class IImageEditorTool;
//-----------------------------------------------------------------------------
class ToolsManager : public QObject
{
    Q_OBJECT
public:
    explicit ToolsManager(QObject *parent = 0);
    ~ToolsManager();

private:
    QList <IImageEditorTool *> *mTools;

    void InitializeTools();

signals:

public slots:
};
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
#endif // TOOLSMANAGER_H
