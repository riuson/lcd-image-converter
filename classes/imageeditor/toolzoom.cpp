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

#include "toolzoom.h"
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QSpinBox>
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolZoom::ToolZoom(QObject *parent) : QObject(parent)
{
    QSvgRenderer renderer(QString(":/images/icons/tools/tool_zoom.svg"), this);

    QImage image(16, 16, QImage::Format_ARGB32);
    image.fill(0x00ffffff);
    QPainter painter(&image);
    renderer.render(&painter);
    this->mIcon = new QIcon(QPixmap::fromImage(image));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();

    this->mZoom = 1;

    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolZoom::~ToolZoom()
{
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolZoom::title() const
{
    return tr("Zoom");
}
//-----------------------------------------------------------------------------
const QString ToolZoom::tooltip() const
{
    return tr("Zoom image");
}
//-----------------------------------------------------------------------------
const QIcon *ToolZoom::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolZoom::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolZoom::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
void ToolZoom::mousePress(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
void ToolZoom::mouseMove(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
void ToolZoom::mouseRelease(const QMouseEvent *event)
{

}
//-----------------------------------------------------------------------------
void ToolZoom::changeZoom(int value)
{
    if (this->mZoom != value)
    {
        this->mZoom = value;
        this->mSpinBoxZoom->setValue(value);
    }
}
//-----------------------------------------------------------------------------
void ToolZoom::initializeWidgets()
{
    {
        this->mSpinBoxZoom = new QSpinBox();
        this->mSpinBoxZoom->setMinimum(1);
        this->mSpinBoxZoom->setSuffix(QString("x"));
        this->mSpinBoxZoom->setValue(this->mZoom);
        this->connect(this->mSpinBoxZoom, SIGNAL(valueChanged(int)), SLOT(on_spinBoxZoom_valueChanged(int)));

        this->mWidgets->append(this->mSpinBoxZoom);
    }
}
//-----------------------------------------------------------------------------
void ToolZoom::on_spinBoxZoom_valueChanged(int value)
{
    if (value != this->mZoom)
    {
        this->mZoom = value;
        emit this->zoomChanged(value);
    }
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
