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

#include "toolcolor.h"
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
#include "iimageeditorparams.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
ToolColor::ToolColor(IImageEditorParams *parameters, QObject *parent) : QObject(parent)
{
    this->mParameters = parameters;
    this->mIcon = new QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_color"), 24)));

    this->mActions = new QList<QAction *>();
    this->mWidgets = new QList<QWidget *>();

    this->loadSettings();
    this->initializeWidgets();
}
//-----------------------------------------------------------------------------
ToolColor::~ToolColor()
{
    this->saveSettings();
    delete this->mIcon;
    qDeleteAll(*this->mActions);
    qDeleteAll(*this->mWidgets);
    delete this->mActions;
    delete this->mWidgets;
}
//-----------------------------------------------------------------------------
const QString ToolColor::title() const
{
    return tr("Color");
}
//-----------------------------------------------------------------------------
const QString ToolColor::tooltip() const
{
    return tr("Select color");
}
//-----------------------------------------------------------------------------
const QIcon *ToolColor::icon() const
{
    return this->mIcon;
}
//-----------------------------------------------------------------------------
const QList<QAction *> *ToolColor::actions() const
{
    return this->mActions;
}
//-----------------------------------------------------------------------------
const QList<QWidget *> *ToolColor::widgets() const
{
    return this->mWidgets;
}
//-----------------------------------------------------------------------------
const QColor ToolColor::foreColor() const
{
    return this->mForeColor;
}
//-----------------------------------------------------------------------------
const QColor ToolColor::backColor() const
{
    return this->mBackColor;
}
//-----------------------------------------------------------------------------
bool ToolColor::processMouse(QMouseEvent *event,
                           const QImage *imageOriginal)
{
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
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
                    QColor color = BitmapHelper::fromRgba(imageOriginal->pixel(event->x(), event->y()));
                    this->mForeColor = color;
                    this->updateColorIcons();
                }

                if(buttonRight)
                {
                    QColor color = BitmapHelper::fromRgba(imageOriginal->pixel(event->x(), event->y()));
                    this->mBackColor = color;
                    this->updateColorIcons();
                }
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
    }

    return true;
}
//-----------------------------------------------------------------------------
void ToolColor::initializeWidgets()
{
    this->mActionForeColor = new QAction(this);
    this->mActionForeColor->setText(tr("Fore Color"));
    this->mActionForeColor->setToolTip(tr("Fore Color"));
    this->connect(this->mActionForeColor, SIGNAL(triggered()), SLOT(on_buttonForeColor_triggered()));
    this->mActions->append(this->mActionForeColor);

    this->mActionBackColor = new QAction(this);
    this->mActionBackColor->setText(tr("Back Color"));
    this->mActionBackColor->setToolTip(tr("Back Color"));
    this->connect(this->mActionBackColor, SIGNAL(triggered()), SLOT(on_buttonBackColor_triggered()));
    this->mActions->append(this->mActionBackColor);

    this->mActionSwapColors = new QAction(this);
    this->mActionSwapColors->setText(tr("Swap Colors"));
    this->mActionSwapColors->setToolTip(tr("Swap Colors"));
    this->connect(this->mActionSwapColors, SIGNAL(triggered()), SLOT(on_buttonSwapColors_triggered()));
    this->mActionSwapColors->setIcon(QIcon(QPixmap::fromImage(BitmapHelper::fromSvg(QString(":/images/icons/tools/tool_color_swap"), 24))));
    this->mActions->append(this->mActionSwapColors);

    this->updateColorIcons();
}
//-----------------------------------------------------------------------------
void ToolColor::loadSettings()
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("color");

    bool ok;
    unsigned int a = sett.value("foreColor", QVariant("none")).toUInt(&ok);

    if (ok)
    {
        this->mForeColor = BitmapHelper::fromRgba(QRgb(a));
    }
    else
    {
        this->mForeColor = QColor("black");
    }

    a = sett.value("backColor", QVariant("none")).toUInt(&ok);

    if (ok)
    {
        this->mBackColor = BitmapHelper::fromRgba(QRgb(a));
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
void ToolColor::saveSettings() const
{
    QSettings sett;
    sett.beginGroup("window-image-editor");
    sett.beginGroup("tools");
    sett.beginGroup("color");

    unsigned int a = this->mForeColor.rgba();
    sett.setValue("foreColor", QVariant(a));

    a = this->mBackColor.rgba();
    sett.setValue("backColor", QVariant(a));

    sett.endGroup();
    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ToolColor::updateColorIcons()
{
    QPixmap pixmapForeColor = QPixmap(24, 24);
    pixmapForeColor.fill(this->mForeColor);
    this->mActionForeColor->setIcon(QIcon(pixmapForeColor));

    QPixmap pixmapBackColor = QPixmap(24, 24);
    pixmapBackColor.fill(this->mBackColor);
    this->mActionBackColor->setIcon(QIcon(pixmapBackColor));
}
//-----------------------------------------------------------------------------
void ToolColor::on_buttonForeColor_triggered()
{
    QColorDialog dialog(this->mForeColor, this->mParameters->parentWidget());
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mForeColor = dialog.selectedColor();
        this->updateColorIcons();
    }
}
//-----------------------------------------------------------------------------
void ToolColor::on_buttonBackColor_triggered()
{
    QColorDialog dialog(this->mBackColor, this->mParameters->parentWidget());
    dialog.setOption(QColorDialog::ShowAlphaChannel, true);

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mBackColor = dialog.selectedColor();
        this->updateColorIcons();
    }
}
//-----------------------------------------------------------------------------
void ToolColor::on_buttonSwapColors_triggered()
{
    QColor color = this->mForeColor;
    this->mForeColor = this->mBackColor;
    this->mBackColor = color;
    this->updateColorIcons();
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
