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
#include "bitmaphelper.h"
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

    this->mSize = 1;

    this->loadSettings();
    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolPen::~ToolPen()
{
    this->saveSettings();
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
bool ToolPen::processMouse(QMouseEvent *event,
                           const QImage *imageOriginal)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->mFlagChanged = false;
        }

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
                    if (!this->mFlagChanged)
                    {
                        this->mInternalImage = *imageOriginal;
                    }

                    this->drawPixel(event->x(), event->y(), this->mForeColor);
                    this->mFlagChanged = true;
                    emit this->processing(&this->mInternalImage);
                }

                if(buttonRight)
                {
                    if (!this->mFlagChanged)
                    {
                        this->mInternalImage = *imageOriginal;
                    }

                    this->drawPixel(event->x(), event->y(), this->mBackColor);
                    this->mFlagChanged = true;
                    emit this->processing(&this->mInternalImage);
                }
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        emit this->completed(&this->mInternalImage, this->mFlagChanged);
    }

    return true;
}
//-----------------------------------------------------------------------------
void ToolPen::initializeWidgets()
{
    QSpinBox *spinBoxSize = new QSpinBox();
    spinBoxSize->setMinimum(1);
    spinBoxSize->setSuffix(QString("px"));
    spinBoxSize->setValue(this->mSize);
    this->connect(spinBoxSize, SIGNAL(valueChanged(int)), SLOT(on_spinBoxSize_valueChanged(int)));
    this->mWidgets->append(spinBoxSize);

    this->mActionForeColor = new QAction(this);
    this->mActionForeColor->setText(tr("Fore Color"));
    this->connect(this->mActionForeColor, SIGNAL(triggered()), SLOT(on_buttonForeColor_triggered()));
    QPixmap pixmapForeColor = QPixmap(24, 24);
    pixmapForeColor.fill(this->mForeColor);
    this->mActionForeColor->setIcon(QIcon(pixmapForeColor));
    this->mActions->append(this->mActionForeColor);

    this->mActionBackColor = new QAction(this);
    this->mActionBackColor->setText(tr("Back Color"));
    this->connect(this->mActionBackColor, SIGNAL(triggered()), SLOT(on_buttonBackColor_triggered()));
    QPixmap pixmapBackColor = QPixmap(24, 24);
    pixmapBackColor.fill(this->mBackColor);
    this->mActionBackColor->setIcon(QIcon(pixmapBackColor));
    this->mActions->append(this->mActionBackColor);
}
//-----------------------------------------------------------------------------
void ToolPen::loadSettings()
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("pen");

    bool ok;
    unsigned int a = sett.value("foreColor", QVariant("none")).toUInt(&ok);

    if (ok)
    {
        this->mForeColor = QColor(QRgb(a));
    }
    else
    {
        this->mForeColor = QColor("black");
    }

    a = sett.value("backColor", QVariant("none")).toUInt(&ok);

    if (ok)
    {
        this->mBackColor = QColor(QRgb(a));
    }
    else
    {
        this->mBackColor = QColor("white");
    }

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolPen::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("pen");

    unsigned int a = this->mForeColor.rgb();
    sett.setValue("foreColor", QVariant(a));

    a = this->mBackColor.rgb();
    sett.setValue("backColor", QVariant(a));

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolPen::drawPixel(int x, int y, const QColor &color)
{
    QImage image = this->mInternalImage;
    this->mInternalImage = BitmapHelper::drawPixel(&image, x, y, color);
}
//-----------------------------------------------------------------------------
void ToolPen::on_spinBoxSize_valueChanged(int value)
{
    this->mSize = value;
}
//-----------------------------------------------------------------------------
void ToolPen::on_buttonForeColor_triggered()
{
    QColorDialog dialog(this->mForeColor);

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mForeColor = dialog.selectedColor();
        QPixmap pixmapForeColor = QPixmap(24, 24);
        pixmapForeColor.fill(this->mForeColor);
        this->mActionForeColor->setIcon(QIcon(pixmapForeColor));
    }
}
//-----------------------------------------------------------------------------
void ToolPen::on_buttonBackColor_triggered()
{
    QColorDialog dialog(this->mBackColor);

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mBackColor = dialog.selectedColor();
        QPixmap pixmapBackColor = QPixmap(24, 24);
        pixmapBackColor.fill(this->mBackColor);
        this->mActionBackColor->setIcon(QIcon(pixmapBackColor));
    }
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
