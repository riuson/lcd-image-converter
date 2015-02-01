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

#include "windoweditor.h"
#include "ui_windoweditor.h"

#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
#include <QIcon>

#include "bitmaphelper.h"
#include "bitmapeditoroptions.h"
#include "toolsmanager.h"
#include "iimageeditortool.h"
#include "imageeditoroptions.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
WindowEditor::WindowEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowEditor)
{
    ui->setupUi(this);

    this->ui->label->installEventFilter(this);

    this->restoreState(ImageEditorOptions::toolbarsState(), 0);
}
//-----------------------------------------------------------------------------
WindowEditor::~WindowEditor()
{
    ImageEditorOptions::setToolbarsState(this->saveState(0));

    delete ui;
}
//-----------------------------------------------------------------------------
void WindowEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        this->updateImageScaled(this->mTools->scale());
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
bool WindowEditor::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;
    if (event->type() == QEvent::MouseMove || event->type() == QEvent::MouseButtonPress)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            this->mFlagChanged = false;
        }
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        // get coordinates
        int xscaled = me->pos().x();
        int yscaled = me->pos().y();
        int xreal = xscaled / this->mTools->scale();
        int yreal = yscaled / this->mTools->scale();
        if (!this->mImageOriginal.isNull())
        {
            if (xreal < this->mImageOriginal.width() && yreal < this->mImageOriginal.height())
            {
                // show coordinates
                QPoint mouseCoordinates(xreal, yreal);
                emit this->mouseMove(&mouseCoordinates);

                // get buttons
                bool buttonLeft = (me->buttons() & Qt::LeftButton) == Qt::LeftButton;
                bool buttonRight = (me->buttons() & Qt::RightButton) == Qt::RightButton;

                // draw on pixmap
                if (buttonLeft)
                {
                    this->drawPixel(xreal, yreal, this->mColor1);
                    this->mFlagChanged = true;
                }
                if(buttonRight)
                {
                    this->drawPixel(xreal, yreal, this->mColor2);
                    this->mFlagChanged = true;
                }
            }
            else
            {
                QPoint mouseCoordinates(-1, -1);
                emit this->mouseMove(&mouseCoordinates);
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (this->mFlagChanged)
        {
            emit this->imageChanged();
        }
        this->mFlagChanged = false;
    }
    else
    {
        result = QObject::eventFilter(obj, event);
    }
    return result;
}
//-----------------------------------------------------------------------------
void WindowEditor::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
    {
        QPoint point = event->globalPos();
        point = this->mapFromGlobal(point);

        QRect labelRect = this->ui->label->rect();
        QPoint labelPoint = this->ui->label->pos();
        labelRect.moveTo(labelPoint);

        if (labelRect.contains(point.x(), point.y()))
        {
            if (event->orientation() == Qt::Vertical)
            {
                int scale = this->mTools->scale();
                if (event->delta() > 0)
                    scale++;
                else
                    scale--;

                emit this->scaleChanged(scale);
            }
            event->accept();
        }
    }
}
//-----------------------------------------------------------------------------
const QImage *WindowEditor::image() const
{
    const QImage *result = &this->mImageOriginal;
    return result;
}
//-----------------------------------------------------------------------------
void WindowEditor::setImage(const QImage *value)
{
    this->mImageOriginal = QImage(*value);
    this->updateImageScaled(this->mTools->scale());
}
//-----------------------------------------------------------------------------
QColor WindowEditor::color1() const
{
    return this->mColor1;
}
//-----------------------------------------------------------------------------
QColor WindowEditor::color2() const
{
    return this->mColor2;
}
//-----------------------------------------------------------------------------
void WindowEditor::setTools(ToolsManager *tools)
{
    this->mTools = tools;
    QList<QAction *> actions = QList<QAction *> (*this->mTools->toolsActions());
    this->ui->toolBarTools->addActions(actions);
    this->connect(this->mTools, SIGNAL(toolChanged(int)), SLOT(toolChanged(int)));
}
//-----------------------------------------------------------------------------
void WindowEditor::updateImageScaled(int value)
{
    if (!this->mImageOriginal.isNull())
    {
        this->mImageScaled = BitmapHelper::scale(&this->mImageOriginal, value);
        this->mImageScaled = BitmapHelper::drawGrid(&this->mImageScaled, value);
        this->mPixmapScaled = QPixmap::fromImage(this->mImageScaled);

        this->ui->label->setPixmap(this->mPixmapScaled);
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::drawPixel(int x, int y, const QColor &color)
{
    QImage image = this->mImageOriginal;
    this->mImageOriginal = BitmapHelper::drawPixel(&image, x, y, color);
    this->updateImageScaled(this->mTools->scale());
}
//-----------------------------------------------------------------------------
void WindowEditor::on_pushButtonColor1_clicked()
{
    QColorDialog dialog(this->mColor1, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor1 = dialog.selectedColor();
        BitmapEditorOptions::setColor1(this->mColor1);
        this->mPixmapColor1.fill(this->mColor1);
        //this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::on_pushButtonColor2_clicked()
{
    QColorDialog dialog(this->mColor2, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor2 = dialog.selectedColor();
        BitmapEditorOptions::setColor2(this->mColor2);
        this->mPixmapColor2.fill(this->mColor2);
        //this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::setScale(int value)
{
    if (this->mImageOriginal.size() * value != this->mImageScaled.size())
    {
        this->updateImageScaled(value);
    }
}
//-----------------------------------------------------------------------------
void WindowEditor::setColor1(const QColor value)
{
    this->mColor1 = value;
}
//-----------------------------------------------------------------------------
void WindowEditor::setColor2(const QColor value)
{
    this->mColor2 = value;
}
//-----------------------------------------------------------------------------
void WindowEditor::toolChanged(int toolIndex)
{
    this->ui->toolBarOptions->clear();
    this->ui->toolBarOptions->addActions(*this->mTools->tools()->at(toolIndex)->actions());

    for (int i = 0; i < this->mTools->tools()->at(toolIndex)->widgets()->length(); i++)
    {
        QWidget *widget = this->mTools->tools()->at(toolIndex)->widgets()->at(i);

        // toolbar takes ownership on widget
        // so widget will be deleted on toolbar deleting
        QAction *widgetAction = this->ui->toolBarOptions->addWidget(widget);

        widgetAction->setVisible(true);
    }
}
//-----------------------------------------------------------------------------
}
//-----------------------------------------------------------------------------
