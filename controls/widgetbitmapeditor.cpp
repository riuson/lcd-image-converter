/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
#include <QSettings>

#include "bitmaphelper.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
WidgetBitmapEditor::WidgetBitmapEditor(IDataContainer *dataContainer, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBitmapEditor)
{
    ui->setupUi(this);

    QSettings sett;
    sett.beginGroup("setup");
    bool ok;
    this->mScale = sett.value("defaultScale", QVariant(10)).toInt(&ok);
    sett.endGroup();

    this->ui->label->installEventFilter(this);

    this->mColor1 = QColor("black");
    this->mColor2 = QColor("white");

    this->mPixmapColor1 = QPixmap(16, 16);
    this->mPixmapColor2 = QPixmap(16, 16);
    this->mPixmapColor1.fill(this->mColor1);
    this->mPixmapColor2.fill(this->mColor2);
    this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));

    this->mDataContainer = dataContainer;
    this->mImageKey = "default";
    this->createImageScaled(this->mScale);

    this->ui->spinBoxScale->setValue(this->mScale);
    QObject *d = dynamic_cast<QObject *>(this->mDataContainer);
    this->connect(d, SIGNAL(imageChanged(QString)), SLOT(mon_dataContainer_imageChanged(QString)));
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
        this->createImageScaled(this->mScale);
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
        QImage *original = this->mDataContainer->image(this->mImageKey);
        if (xreal < original->width() && yreal < original ->height())
        {
            // show coordinates
            this->ui->labelCoordinates->setText(tr("x: %1, y: %2").arg(xreal).arg(yreal));
            // get buttons
            bool buttonLeft = (me->buttons() & Qt::LeftButton) == Qt::LeftButton;
            bool buttonRight = (me->buttons() & Qt::RightButton) == Qt::RightButton;
            // draw on pixmap
            if (buttonLeft || buttonRight)
            {
                QPainter painterScaled(&this->mPixmapScaled);
                QColor color;
                if (buttonLeft)
                    color = this->mColor1;
                if (buttonRight)
                    color = this->mColor2;

                if (this->mScale == 1)
                {
                    painterScaled.setPen(color);
                    painterScaled.drawPoint(xscaled, yscaled);
                }
                else
                {
                    painterScaled.fillRect(xreal * this->mScale,
                                           yreal * this->mScale,
                                           this->mScale,
                                           this->mScale,
                                           color);
                }

                this->ui->label->setPixmap(this->mPixmapScaled);

                QPainter painterOriginal(original);
                painterOriginal.setPen(color);
                painterOriginal.drawPoint(xreal, yreal);

                this->mFlagChanged = true;
            }
        }
        else
        {
            this->ui->labelCoordinates->setText(tr("Size: %1,%2").arg(original->width()).arg(original->height()));
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
            emit this->dataChanged();
        }
    }
    else
    {
        result = QObject::eventFilter(obj, event);
    }
    return result;
}
//-----------------------------------------------------------------------------
IDataContainer *WidgetBitmapEditor::dataContainer()
{
    return this->mDataContainer;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::selectImage(const QString &key)
{
    this->mImageKey = key;
    this->createImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
const QString WidgetBitmapEditor::currentImageKey()
{
    return this->mImageKey;
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
void WidgetBitmapEditor::createImageScaled(int scale)
{
    QImage *original = this->mDataContainer->image(this->mImageKey);
    if (original != NULL)
    {
        int width = original->width();
        int height = original->height();
        //this->mImageScaled = new QImage(width * scale, height * scale, QImage::Format_RGB32);
        QImage scaled = original->scaled(width * scale, height * scale, Qt::KeepAspectRatio, Qt::FastTransformation);
        this->mPixmapScaled = QPixmap::fromImage(scaled);
        this->ui->label->setPixmap(this->mPixmapScaled);

        this->ui->labelCoordinates->setText(tr("Size: %1,%2").arg(original->width()).arg(original->height()));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_spinBoxScale_valueChanged(int value)
{
    this->mScale = value;
    this->createImageScaled(this->mScale);

    QSettings sett;
    sett.beginGroup("setup");
    sett.setValue("defaultScale", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonColor1_clicked()
{
    QColorDialog dialog(this->mColor1, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor1 = dialog.selectedColor();
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
        this->mPixmapColor2.fill(this->mColor2);
        this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::mon_dataContainer_imageChanged(const QString &key)
{
    if (this->mImageKey == key)
    {
        this->createImageScaled(this->mScale);
        emit this->dataChanged();;
    }
}
//-----------------------------------------------------------------------------
