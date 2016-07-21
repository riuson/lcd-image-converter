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

#include "toolfill.h"
#include <QPainter>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QColor>
#include <QSpinBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QSettings>
#include <QColorDialog>
#include <QVector>
#include "bitmaphelper.h"
#include "iimageeditorparams.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolFill::ToolFill(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
    this->mParameters = parameters;
    this->mIcon = new QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_fill"), 24)));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();

    this->mSize = 1;
    this->mFlagChanged = false;

    this->loadSettings();
    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolFill::~ToolFill()
{
    this->saveSettings();
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolFill::title() const
{
    return tr("Fill");
}
//-----------------------------------------------------------------------------
const QString ToolFill::tooltip() const
{
    return tr("Fill area");
}
//-----------------------------------------------------------------------------
const QIcon *ToolFill::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolFill::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolFill::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
bool ToolFill::processMouse(QMouseEvent *event,
                           const QImage *imageOriginal)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            // get coordinates
            if (imageOriginal != NULL)
            {
                if (event->x() < imageOriginal->width() && event->y() < imageOriginal->height())
                {
                    // get buttons
                    bool buttonLeft = (event->buttons() & Qt::LeftButton) == Qt::LeftButton;
                    bool buttonRight = (event->buttons() & Qt::RightButton) == Qt::RightButton;

                    // draw on pixmap
                    if (buttonLeft)
                    {
                        this->mInternalImage = *imageOriginal;
                        this->mFlagChanged = false;

                        bool success = this->fillArea(event->x(), event->y(), this->mParameters->foreColor());

                        if (success)
                        {
                            this->mFlagChanged = true;
                        }

                        emit this->processing(&this->mInternalImage);
                    }

                    if(buttonRight)
                    {
                        this->mInternalImage = *imageOriginal;
                        this->mFlagChanged = false;

                        bool success = this->fillArea(event->x(), event->y(), this->mParameters->backColor());

                        if (success)
                        {
                            this->mFlagChanged = true;
                        }

                        emit this->processing(&this->mInternalImage);
                    }
                }
            }
            event->accept();
            emit this->completed(&this->mInternalImage, this->mFlagChanged);
        }
    }

    return true;
}
//-----------------------------------------------------------------------------
void ToolFill::initializeWidgets()
{
}
//-----------------------------------------------------------------------------
void ToolFill::loadSettings()
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("fill");

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolFill::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("fill");

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
bool ToolFill::fillArea(int x, int y, const QColor &color)
{
    QImage image = this->mInternalImage;
    QRgb keyColor = image.pixel(x, y);
    QVector <QPoint> vector1;

    this->collectPointsAround(image, QPoint(x, y), keyColor, &vector1);

    int lastLength = 0;
    while (vector1.size() > lastLength)
    {
        lastLength = vector1.size();

        for (int i = 0 ; i < lastLength; i++)
        {
            //painter.drawPoint(vector1.at(i));
            this->collectPointsAround(image, vector1.at(i), keyColor, &vector1);
        }
    }

    if (vector1.size() > 0)
    {
        QPixmap pixmap = QPixmap::fromImage(image);
        QPainter painter(&pixmap);
        painter.setPen(color);

        for (int i = 0 ; i < vector1.size(); i++)
        {
            painter.drawPoint(vector1.at(i));
        }

        image = pixmap.toImage();
        this->mInternalImage = image;
        return true;
    }

    return false;
}
//-----------------------------------------------------------------------------
void ToolFill::collectPointsAround(const QImage &image, const QPoint &point, const QRgb &color, QVector<QPoint> *vector)
{
    int x = point.x();
    int y = point.y();

    //this->collectPoint(image, QPoint(x - 1, y - 1), color, vector);
    this->collectPoint(image, QPoint(x - 0, y - 1), color, vector);
    //this->collectPoint(image, QPoint(x + 1, y - 1), color, vector);
    this->collectPoint(image, QPoint(x - 1, y - 0), color, vector);
    this->collectPoint(image, QPoint(x - 0, y - 0), color, vector);
    this->collectPoint(image, QPoint(x + 1, y - 0), color, vector);
    //this->collectPoint(image, QPoint(x - 1, y + 1), color, vector);
    this->collectPoint(image, QPoint(x - 0, y + 1), color, vector);
    //this->collectPoint(image, QPoint(x + 1, y + 1), color, vector);
}
//-----------------------------------------------------------------------------
void ToolFill::collectPoint(const QImage &image, const QPoint &point, const QRgb &color, QVector<QPoint> *vector)
{
    if (point.x() >= 0 && point.y() >= 0 && point.x() < image.width() && point.y() < image.height())
    {
        if (!vector->contains(point))
        {
            if (image.pixel(point) == color)
            {
                vector->append(point);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ToolFill::on_spinBoxSize_valueChanged(int value)
{
    this->mSize = value;
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
