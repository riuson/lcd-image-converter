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

    this->mPixmapOriginal = QPixmap();

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
        this->updatePixmapScaled(this->mScale);
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
        if (!this->mPixmapOriginal.isNull())
        {
            if (xreal < this->mPixmapOriginal.width() && yreal < this->mPixmapOriginal.height())
            {
                // show coordinates
                this->ui->labelCoordinates->setText(tr("x: %1, y: %2").arg(xreal).arg(yreal));

                // get buttons
                bool buttonLeft = (me->buttons() & Qt::LeftButton) == Qt::LeftButton;
                bool buttonRight = (me->buttons() & Qt::RightButton) == Qt::RightButton;

                // draw on pixmap
                if (buttonLeft || buttonRight)
                {
                    QColor color;
                    if (buttonLeft)
                        color = this->mColor1;
                    if (buttonRight)
                        color = this->mColor2;

                    this->drawPixel(xreal, yreal, color);

                    this->mFlagChanged = true;
                }
            }
            else
            {
                this->ui->labelCoordinates->setText(tr("%1 x %2").arg(this->mPixmapOriginal.width()).arg(this->mPixmapOriginal.height()));
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
const QImage WidgetBitmapEditor::currentImage() const
{
    QImage result = this->mPixmapOriginal.toImage();
    return result;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::setCurrentImage(const QImage &value)
{
    if (value.isNull())
    {
        this->mPixmapOriginal = QPixmap();
    }
    else
    {
        this->mPixmapOriginal = QPixmap::fromImage(value);
    }
    this->updatePixmapScaled(this->mScale);
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
void WidgetBitmapEditor::updatePixmapScaled(int scale)
{
    if (!this->mPixmapOriginal.isNull())
    {
        this->mPixmapScaled = BitmapHelper::createPixmapScaled(this->mPixmapOriginal, scale);

        this->ui->label->setPixmap(this->mPixmapScaled);

        this->ui->labelCoordinates->setText(tr("%1 x %2").arg(this->mPixmapOriginal.width()).arg(this->mPixmapOriginal.height()));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_spinBoxScale_valueChanged(int value)
{
    this->mScale = value;
    this->updatePixmapScaled(this->mScale);

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
void WidgetBitmapEditor::mon_dataContainer_imageChanged(const QString &key)
{
    //if (this->mImageKey == key)
    {
        this->updatePixmapScaled(this->mScale);
        emit this->imageChanged();
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::drawPixel(int x, int y, const QColor &color)
{
    QPainter painterScaled(&this->mPixmapScaled);

    if (this->mScale == 1)
    {
        painterScaled.setPen(color);
        painterScaled.drawPoint(x * this->mScale, y * this->mScale);
    }
    else
    {
        painterScaled.fillRect(x * this->mScale + 1,
                               y * this->mScale + 1,
                               this->mScale - 1,
                               this->mScale - 1,
                               color);
        BitmapHelper::drawGrid(this->mPixmapOriginal, this->mPixmapScaled, &painterScaled, this->mScale);
    }

    this->ui->label->setPixmap(this->mPixmapScaled);

    QPainter painterOriginal(&this->mPixmapOriginal);
    painterOriginal.setPen(color);
    painterOriginal.drawPoint(x, y);
}
//-----------------------------------------------------------------------------
