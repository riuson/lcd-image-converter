/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#include "dialogpreview.h"
#include "ui_dialogpreview.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QRegExp>
#include <QFont>
#include "datacontainer.h"
#include "converterhelper.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
DialogPreview::DialogPreview(DataContainer *dataContainer, Preset *matrix, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreview)
{
    ui->setupUi(this);

    this->mData = dataContainer;
    this->mPreset = matrix;
    this->mScale = 1;

    if (this->mData != NULL)
    {
        this->ui->comboBoxSampleKey->addItems(this->mData->keys());
        this->ui->comboBoxSampleKey->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBoxSampleKey->setEnabled(false);
    }

    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    this->ui->plainTextEdit->setFont(font);
}
//-----------------------------------------------------------------------------
DialogPreview::~DialogPreview()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogPreview::updatePreview()
{
    if (this->mData != NULL)
    {
        QString key = this->ui->comboBoxSampleKey->currentText();
        if (!key.isEmpty())
        {
            this->mImageOriginal = QImage(*this->mData->image(key));
            QImage processed;
            ConverterHelper::createImagePreview(this->mPreset, &this->mImageOriginal, &processed);
            //this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));
            this->mImageProcessed = processed;
            this->updatePreviewScaled(&this->mImageProcessed, this->mScale);

            //BitmapData data;
            //this->mConverter->processImage(processed, &data);

            //QString str = this->mConverter->dataToString(data);
            //this->ui->plainTextEdit->setPlainText(str);

            QVector<quint32> sourceData;
            int sourceWidth, sourceHeight;
            ConverterHelper::pixelsData(this->mPreset, &this->mImageProcessed, &sourceData, &sourceWidth, &sourceHeight);

            ConverterHelper::processPixels(this->mPreset, &sourceData);

            QVector<quint32> packedData;
            int packedWidth, packedHeight;
            ConverterHelper::packData(this->mPreset, &sourceData, sourceWidth, sourceHeight, &packedData, &packedWidth, &packedHeight);

            QVector<quint32> reorderedData;
            int reorderedWidth, reorderedHeight;
            ConverterHelper::reorder(this->mPreset, &packedData, packedWidth, packedHeight, &reorderedData, &reorderedWidth, &reorderedHeight);

            QVector<quint32> compressedData;
            int compressedWidth, compressedHeight;
            ConverterHelper::compressData(this->mPreset, &reorderedData, reorderedWidth, reorderedHeight, &compressedData, &compressedWidth, &compressedHeight);

            QString dataString = ConverterHelper::dataToString(
                        this->mPreset,
                        &compressedData, compressedWidth, compressedHeight);

            this->ui->plainTextEdit->setPlainText(dataString);
        }
    }
}
//-----------------------------------------------------------------------------
void DialogPreview::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) == Qt::ControlModifier)
    {
        QPoint point = event->globalPos();
        point = this->mapFromGlobal(point);

        QRect labelRectPreview = this->ui->labelPreview->rect();
        QPoint labelPoint = this->ui->labelPreview->pos();
        labelRectPreview.moveTo(labelPoint);

        if (labelRectPreview.contains(point.x(), point.y()))
        {
            if (event->orientation() == Qt::Vertical)
            {
                int scale = this->mScale;
                if (event->delta() > 0)
                    scale++;
                else
                    scale--;

                this->setScale(scale);
                this->ui->spinBoxScale->setValue(this->mScale);
            }
            event->accept();
        }
    }
}
//-----------------------------------------------------------------------------
void DialogPreview::setScale(int value)
{
    if (this->mScale != value)
    {
        if (value > 0 && value <= 50)
        {
            this->mScale = value;
            this->updatePreviewScaled(&this->mImageProcessed, this->mScale);
        }
    }
}
//-----------------------------------------------------------------------------
void DialogPreview::updatePreviewScaled(const QImage *image, int scale)
{
    QImage imageScaled = BitmapHelper::scale(image, scale);
    imageScaled = BitmapHelper::drawGrid(&imageScaled, scale);
    QPixmap pixmapScaled = QPixmap::fromImage(imageScaled);
    this->ui->labelPreview->setPixmap(pixmapScaled);
}
//-----------------------------------------------------------------------------
void DialogPreview::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogPreview::on_spinBoxScale_valueChanged(int value)
{
    this->setScale(value);
}
//-----------------------------------------------------------------------------
