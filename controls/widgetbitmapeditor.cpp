/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#include "widgetbitmapeditor.h"
#include "ui_widgetbitmapeditor.h"
//-----------------------------------------------------------------------------
#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>

#include "bitmaphelper.h"
#include "bitmapeditoroptions.h"
//-----------------------------------------------------------------------------
WidgetBitmapEditor::WidgetBitmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBitmapEditor)
{
    ui->setupUi(this);

    this->mScale = BitmapEditorOptions::scale();

    this->ui->label->installEventFilter(this);

    this->mColor1 = BitmapEditorOptions::color1();
    this->mColor2 = BitmapEditorOptions::color2();

    this->mPixmapColor1 = QPixmap(16, 16);
    this->mPixmapColor2 = QPixmap(16, 16);
    this->mPixmapColor1.fill(this->mColor1);
    this->mPixmapColor2.fill(this->mColor2);
    this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));

    this->mImageOriginal = QImage();

    this->ui->spinBoxScale->setValue(this->mScale);
}
//-----------------------------------------------------------------------------
WidgetBitmapEditor::~WidgetBitmapEditor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        this->updateImageScaled(this->mScale);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
bool WidgetBitmapEditor::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        // get coordinates
        int xscaled = me->pos().x();
        int yscaled = me->pos().y();
        int xreal = xscaled / this->mScale;
        int yreal = yscaled / this->mScale;
        if (!this->mImageOriginal.isNull())
        {
            if (xreal < this->mImageOriginal.width() && yreal < this->mImageOriginal.height())
            {
                // show coordinates
                this->ui->labelCoordinates->setText(tr("x: %1, y: %2").arg(xreal).arg(yreal));

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
                this->ui->labelCoordinates->setText(tr("%1 x %2").arg(this->mImageOriginal.width()).arg(this->mImageOriginal.height()));
            }
        }
        event->accept();
    }
    else if (event->type() == QEvent::MouseButtonPress)
    {
        this->mFlagChanged = false;
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
        if (this->mFlagChanged)
        {
            emit this->imageChanged();
        }
    }
    else
    {
        result = QObject::eventFilter(obj, event);
    }
    return result;
}
//-----------------------------------------------------------------------------
const QImage *WidgetBitmapEditor::image() const
{
    const QImage *result = &this->mImageOriginal;
    return result;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::setImage(const QImage *value)
{
    this->mImageOriginal = QImage(*value);
    this->updateImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
QColor WidgetBitmapEditor::color1()
{
    return this->mColor1;
}
//-----------------------------------------------------------------------------
QColor WidgetBitmapEditor::color2()
{
    return this->mColor2;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::updateImageScaled(int scale)
{
    if (!this->mImageOriginal.isNull())
    {
        this->mImageScaled = BitmapHelper::scale(&this->mImageOriginal, scale);
        this->mImageScaled = BitmapHelper::drawGrid(&this->mImageScaled, scale);
        this->mPixmapScaled = QPixmap::fromImage(this->mImageScaled);

        this->ui->label->setPixmap(this->mPixmapScaled);

        this->ui->labelCoordinates->setText(tr("%1 x %2").arg(this->mImageOriginal.width()).arg(this->mImageOriginal.height()));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_spinBoxScale_valueChanged(int value)
{
    this->mScale = value;
    this->updateImageScaled(this->mScale);

    BitmapEditorOptions::setScale(value);
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonColor1_clicked()
{
    QColorDialog dialog(this->mColor1, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor1 = dialog.selectedColor();
        BitmapEditorOptions::setColor1(this->mColor1);
        this->mPixmapColor1.fill(this->mColor1);
        this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonColor2_clicked()
{
    QColorDialog dialog(this->mColor2, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor2 = dialog.selectedColor();
        BitmapEditorOptions::setColor2(this->mColor2);
        this->mPixmapColor2.fill(this->mColor2);
        this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::drawPixel(int x, int y, const QColor &color)
{
    QImage image = this->mImageOriginal;
    this->mImageOriginal = BitmapHelper::drawPixel(&image, x, y, color);
    this->updateImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
