/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
#include "idatacontainer.h"
#include "converterhelper.h"
#include <QList>
//-----------------------------------------------------------------------------
DialogPreview::DialogPreview(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreview)
{
    ui->setupUi(this);

    this->mData = dataContainer;

    if (this->mData != NULL)
    {
        this->ui->comboBoxSampleKey->addItems(this->mData->keys());
        this->ui->comboBoxSampleKey->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBoxSampleKey->setEnabled(false);
    }
}
//-----------------------------------------------------------------------------
DialogPreview::~DialogPreview()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogPreview::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
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
            this->ui->labelOriginal->setPixmap(QPixmap::fromImage(this->mImageOriginal));
            //QImage processed = this->mConverter->preprocessImage(this->mImageOriginal);
            //this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));

            //BitmapData data;
            //this->mConverter->processImage(processed, &data);

            //QString str = this->mConverter->dataToString(data);
            //this->ui->plainTextEdit->setPlainText(str);

            QList<quint32> matrix;
            ConverterHelper::createMatrixColor(&matrix);

            QList<quint32> data;
            int width, height;
            ConverterHelper::pixelsData(&matrix, &this->mImageOriginal, &data, &width, &height);

            ConverterHelper::processPixels(&matrix, &data);

            QList<quint32> data2;
            int width2, height2;
            ConverterHelper::packData(&matrix, &data, width, height, &data2, &width2, &height2);
        }
    }
}
//-----------------------------------------------------------------------------
