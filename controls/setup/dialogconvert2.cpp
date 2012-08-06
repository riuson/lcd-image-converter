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
#include "sourcepreviewmodel.h"
#include "operationspreviewmodel.h"
#include "maskpreviewmodel.h"
//-----------------------------------------------------------------------------
DialogConvert2::DialogConvert2(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert2)
{
    ui->setupUi(this);

    this->mData = dataContainer;

    this->mMatrix = new QList<quint32>();

    //ConverterHelper::createMatrixMono(this->mMatrix);
    //ConverterHelper::createMatrixGrayscale(this->mMatrix);
    ConverterHelper::createMatrixColor(this->mMatrix);

    this->mSourceModel = new SourcePreviewModel(this->mMatrix, this);
    this->ui->tableViewSource->setModel(this->mSourceModel);
    this->setTableHeight(this->ui->tableViewSource);

    this->mOperationsModel = new OperationsPreviewModel(this->mMatrix, this);
    this->ui->tableViewOperations->setModel(this->mOperationsModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();

    this->mMaskModelUsed = new MaskPreviewModel(this->mMatrix, 1, this);
    this->ui->tableViewUsed->setModel(this->mMaskModelUsed);
    this->setTableHeight(this->ui->tableViewUsed);

    this->mMaskModelAnd = new MaskPreviewModel(this->mMatrix, 2, this);
    this->ui->tableViewAnd->setModel(this->mMaskModelAnd);
    this->setTableHeight(this->ui->tableViewAnd);

    this->mMaskModelOr = new MaskPreviewModel(this->mMatrix, 3, this);
    this->ui->tableViewOr->setModel(this->mMaskModelOr);
    this->setTableHeight(this->ui->tableViewOr);
}
//-----------------------------------------------------------------------------
DialogConvert2::~DialogConvert2()
{
    delete ui;
    delete this->mMaskModelOr;
    delete this->mMaskModelAnd;
    delete this->mMaskModelUsed;
    delete this->mOperationsModel;
    delete this->mSourceModel;
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
void DialogConvert2::on_pushButtonPreview_clicked()
{
    DialogPreview preview(this->mData, this);
    preview.exec();
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
