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
#include <QRegExp>
//-----------------------------------------------------------------------------
DialogPreview::DialogPreview(IDataContainer *dataContainer, QList<quint32> *matrix, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreview)
{
    ui->setupUi(this);

    this->mData = dataContainer;
    this->mMatrix = matrix;

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

            QList<quint32> data;
            int width, height;
            ConverterHelper::pixelsData(this->mMatrix, &this->mImageOriginal, &data, &width, &height);

            ConverterHelper::processPixels(this->mMatrix, &data);

            QList<quint32> data2;
            int width2, height2;
            ConverterHelper::packData(this->mMatrix, &data, width, height, &data2, &width2, &height2);

            ConversionMatrixOptions options(this->mMatrix);

            QString str;
            int fieldWidth = ((int)options.blockSize() + 1) << 1;
            for (int y = 0; y < height2; y++)
            {
                if (str.endsWith(", "))
                    str.append("\n");
                for (int x = 0; x < width2; x++)
                {
                    str += QString("%1, ").arg(data2.at(y * width2 + x), fieldWidth, 16, QChar('0'));
                }
            }
            if (str.endsWith(", "))
                str = str.remove(QRegExp("\\,\\s$"));

            this->ui->plainTextEdit->setPlainText(str);
        }
    }
}
//-----------------------------------------------------------------------------
void DialogPreview::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
