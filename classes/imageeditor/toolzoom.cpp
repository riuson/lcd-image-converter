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
#include <QSettings>
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

    this->mScale = 1;

    this->loadSettings();
    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolZoom::~ToolZoom()
{
    this->saveSettings();
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
int ToolZoom::scale() const
{
    return this->mScale;
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
void ToolZoom::setScale(int value)
{
    if (this->mScale != value && value >= this->mSpinBoxScale->minimum() && value <= this->mSpinBoxScale->maximum())
    {
        this->mScale = value;
        this->mSpinBoxScale->setValue(value);
    }
}
//-----------------------------------------------------------------------------
void ToolZoom::initializeWidgets()
{
    {
        this->mSpinBoxScale = new QSpinBox();
        this->mSpinBoxScale->setMinimum(1);
        this->mSpinBoxScale->setMaximum(50);
        this->mSpinBoxScale->setSuffix(QString("x"));
        this->mSpinBoxScale->setValue(this->mScale);
        this->connect(this->mSpinBoxScale, SIGNAL(valueChanged(int)), SLOT(on_spinBoxScale_valueChanged(int)));

        this->mWidgets->append(this->mSpinBoxScale);
    }
}
//-----------------------------------------------------------------------------
void ToolZoom::loadSettings()
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("zoom");

    bool ok;
    int value = sett.value("scale", QVariant(1)).toInt(&ok);

    if (ok)
    {
        this->mScale = value;
    }

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolZoom::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("zoom");

    sett.setValue("scale", this->mScale);

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolZoom::on_spinBoxScale_valueChanged(int value)
{
    if (value != this->mScale)
    {
        this->mScale = value;
        emit this->scaleChanged(value);
    }
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
