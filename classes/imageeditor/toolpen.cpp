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
#include <QtSvg/QSvgRenderer>
#include <QPixmap>
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolPen::ToolPen(QObject *parent) : QObject(parent)
{
    QSvgRenderer renderer(QString(":/images/icons/tools/tool_pen.svg"), this);

    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(0x00ffffff);
    QPainter painter(&image);
    renderer.render(&painter);
    this->mIcon = new QIcon(QPixmap::fromImage(image));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();
}
//-----------------------------------------------------------------------------
ToolPen::~ToolPen()
{
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolPen::title() const
{
    return tr("Pen");
}
//-----------------------------------------------------------------------------
const QString ToolPen::tooltip() const
{
    return tr("Draw pixels");
}
//-----------------------------------------------------------------------------
const QIcon *ToolPen::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolPen::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolPen::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
void ToolPen::mousePress(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
void ToolPen::mouseMove(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
void ToolPen::mouseRelease(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
