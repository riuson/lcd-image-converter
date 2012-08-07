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

#include "dialogconvert2.h"
#include "ui_dialogconvert2.h"
//-----------------------------------------------------------------------------
#include <QList>
#include "idatacontainer.h"
#include "converterhelper.h"
#include "dialogpreview.h"
#include "matrixpreviewmodel.h"
#include "conversionmatrixoptions.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
DialogConvert2::DialogConvert2(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert2)
{
    ui->setupUi(this);

    this->mData = dataContainer;
    this->mMatrix = new QList<quint32>();

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(tr("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxBlockSize->addItem(tr("8 bit"), Data8);
    this->ui->comboBoxBlockSize->addItem(tr("16 bit"), Data16);
    this->ui->comboBoxBlockSize->addItem(tr("24 bit"), Data24);
    this->ui->comboBoxBlockSize->addItem(tr("32 bit"), Data32);

    this->ui->comboBoxRotate->addItem(tr("Rotate none"), QVariant(BitmapHelper::TransformNone));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Clockwise"), QVariant(BitmapHelper::TransformRotate90));
    this->ui->comboBoxRotate->addItem(tr("Rotate 180"), QVariant(BitmapHelper::TransformRotate180));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Counter-Clockwise"), QVariant(BitmapHelper::TransformRotate270));

    //ConverterHelper::createMatrixMono(this->mMatrix);
    //ConverterHelper::createMatrixGrayscale(this->mMatrix);
    ConverterHelper::createMatrixColor(this->mMatrix);

    this->mMatrixModel = new MatrixPreviewModel(this->mMatrix, this);
    this->ui->tableViewOperations->setModel(this->mMatrixModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();
}
//-----------------------------------------------------------------------------
DialogConvert2::~DialogConvert2()
{
    delete ui;
    delete this->mMatrixModel;
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
void DialogConvert2::setTableHeight(QTableView *tableView)
{
    tableView->resizeColumnsToContents();
    tableView->resizeRowsToContents();
    tableView->setMaximumHeight(
                tableView->verticalHeader()->length() +
                tableView->horizontalHeader()->height() +
                tableView->horizontalScrollBar()->height());
}
//-----------------------------------------------------------------------------
void DialogConvert2::updatePreview()
{
    if (this->mData != NULL)
    {
        /*QString key = this->ui->comboBoxSampleKey->currentText();
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
        }*/
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_pushButtonPreview_clicked()
{
    DialogPreview preview(this->mData, this);
    preview.exec();
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxConversionType_currentIndexChanged()
{
    QVariant data = this->ui->comboBoxConversionType->itemData(this->ui->comboBoxConversionType->currentIndex());
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setConvType((ConversionType)a);

        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxMonochromeType_currentIndexChanged()
{
    QVariant data = this->ui->comboBoxMonochromeType->itemData(this->ui->comboBoxMonochromeType->currentIndex());
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setMonoType((MonochromeType)a);

        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxBlockSize_currentIndexChanged()
{
    QVariant data = this->ui->comboBoxBlockSize->itemData(this->ui->comboBoxBlockSize->currentIndex());
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setBlockSize((DataBlockSize)a);

        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();
    }
}
//-----------------------------------------------------------------------------
