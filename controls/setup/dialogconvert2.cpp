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
#include <QSettings>
#include <QStringList>
#include <QInputDialog>
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

    this->ui->comboBoxRotate->addItem(tr("Rotate none"), QVariant(TransformNone));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Clockwise"), QVariant(TransformRotate90));
    this->ui->comboBoxRotate->addItem(tr("Rotate 180"), QVariant(TransformRotate180));
    this->ui->comboBoxRotate->addItem(tr("Rotate 90 Counter-Clockwise"), QVariant(TransformRotate270));

    //ConverterHelper::createMatrixMono(this->mMatrix);
    //ConverterHelper::createMatrixGrayscale(this->mMatrix);
    ConverterHelper::createMatrixColor(this->mMatrix);

    this->mMatrixModel = new MatrixPreviewModel(this->mMatrix, this);
    this->ui->tableViewOperations->setModel(this->mMatrixModel);
    this->ui->tableViewOperations->resizeColumnsToContents();
    this->ui->tableViewOperations->resizeRowsToContents();

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
DialogConvert2::~DialogConvert2()
{
    delete ui;
    delete this->mMatrixModel;
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
void DialogConvert2::fillPresetsList()
{
    QString current = this->ui->comboBoxPresets->currentText();

    this->ui->comboBoxPresets->clear();

    QSettings sett;
    sett.beginGroup("presets");
    QStringList names = sett.childGroups();
    sett.endGroup();

    this->ui->comboBoxPresets->addItems(names);

    if (names.contains(current))
    {
        this->ui->comboBoxPresets->setCurrentIndex(names.indexOf(current));
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::presetLoad(const QString &name)
{
    if (name.isEmpty())
        return;

    QSettings sett;
    sett.beginGroup("presets");

    if (sett.childGroups().contains(name))
    {
        sett.beginGroup(name);

        QString strFlags = sett.value("flags", QString("00000000")).toString();
        QString strMaskUsed = sett.value("maskUsed", QString("ffffffff")).toString();
        QString strMaskAnd = sett.value("maskAnd", QString("ffffffff")).toString();
        QString strMaskOr = sett.value("maskOr", QString("00000000")).toString();


        bool ok;
        quint32 flags, maskUsed, maskAnd, maskOr;
        flags = strFlags.toUInt(&ok, 16);
        if (ok)
        {
            maskUsed = strMaskUsed.toUInt(&ok, 16);

            if (ok)
            {
                maskAnd = strMaskAnd.toUInt(&ok, 16);
                if (ok)
                {
                    maskOr = strMaskOr.toUInt(&ok, 16);

                    this->mMatrix->clear();
                    this->mMatrix->append(flags);
                    this->mMatrix->append(maskUsed);
                    this->mMatrix->append(maskAnd);
                    this->mMatrix->append(maskOr);

                    int operations = sett.beginReadArray("matrix");

                    for (int i = 0; i < operations; i++)
                    {
                        sett.setArrayIndex(i);

                        QString strMask = sett.value("mask", QString("00000000")).toString();
                        QString strShift = sett.value("shift", QString("00000000")).toString();
                        quint32 mask, shift;
                        if (ok)
                        {
                            mask = strMask.toUInt(&ok, 16);

                            if (ok)
                            {
                                shift = strShift.toUInt(&ok, 16);
                                if (ok)
                                {
                                    this->mMatrix->append(mask);
                                    this->mMatrix->append(shift);
                                }
                            }
                        }
                    }

                    sett.endArray();;
                }

            }

        }

        sett.endGroup();
    }
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void DialogConvert2::presetSaveAs(const QString &name)
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    int operations = this->mMatrix->length() - ConversionMatrixOptions::OperationsStartIndex;
    operations = operations >> 1;

    sett.setValue("flags", QString("%1").arg(this->mMatrix->at(0), 8, 16, QChar('0')));
    sett.setValue("maskUsed", QString("%1").arg(this->mMatrix->at(1), 8, 16, QChar('0')));
    sett.setValue("maskAnd", QString("%1").arg(this->mMatrix->at(2), 8, 16, QChar('0')));
    sett.setValue("maskOr", QString("%1").arg(this->mMatrix->at(3), 8, 16, QChar('0')));

    sett.beginWriteArray("matrix");

    for (int i = 0; i < operations; i++)
    {
        sett.setArrayIndex(i);
        sett.setValue("mask", QString("%1").arg(this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (i << 1)), 8, 16, QChar('0')));
        sett.setValue("shift", QString("%1").arg(this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (i << 1) + 1), 8, 16, QChar('0')));
    }
    sett.endArray();

    sett.endGroup();
    sett.endGroup();

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert2::presetRemove(const QString &name)
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    sett.endGroup();

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert2::updatePreview()
{
    if (this->mData != NULL)
    {
        this->ui->tableViewOperations->update();
        this->ui->tableViewOperations->resizeColumnsToContents();
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
void DialogConvert2::on_comboBoxConversionType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxConversionType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setConvType((ConversionType)a);

        this->updatePreview();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxMonochromeType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxMonochromeType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setMonoType((MonochromeType)a);

        this->updatePreview();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxBlockSize_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxBlockSize->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        options.setBlockSize((DataBlockSize)a);

        this->updatePreview();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxRotate_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxRotate->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ConversionMatrixOptions options(this->mMatrix);
        //BitmapHelper::BitmapHelperTransformCodes rotate = (BitmapHelper::BitmapHelperTransformCodes)a;
        quint32 b = options.transform();
        b = b & ~(TransformRotatations);
        b = b | a;
        options.setTransform((Transformation)b);

        this->updatePreview();
    }
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_checkBoxFlipHorizontal_toggled(bool value)
{
    ConversionMatrixOptions options(this->mMatrix);
    int a = options.transform();
    if (value)
        a = a | TransformFlipHorizontal;
    else
        a = a & ~(TransformFlipHorizontal);
    options.setTransform((Transformation)a);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_checkBoxFlipVertical_toggled(bool value)
{
    ConversionMatrixOptions options(this->mMatrix);
    int a = options.transform();
    if (value)
        a = a | TransformFlipVertical;
    else
        a = a & ~(TransformFlipVertical);
    options.setTransform((Transformation)a);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_checkBoxInverse_toggled(bool value)
{
    ConversionMatrixOptions options(this->mMatrix);
    int a = options.transform();
    if (value)
        a = a | TransformInverse;
    else
        a = a & ~(TransformInverse);
    options.setTransform((Transformation)a);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_pushButtonPresetSaveAs_clicked()
{
    QSettings sett;
    sett.beginGroup("presets");

    QStringList names = sett.childGroups();

    sett.endGroup();

    QInputDialog dialog(this);
    dialog.setComboBoxItems(names);
    dialog.setComboBoxEditable(true);

    QString current = this->ui->comboBoxPresets->currentText();
    bool ok;

    QString result = dialog.getItem(this, tr("Enter preset name"), tr("Preset name:"), names, names.indexOf(current), true, &ok);
    if (ok && !result.isEmpty())
    {
        this->presetSaveAs(result);
    }

    this->fillPresetsList();
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_pushButtonPresetRemove_clicked()
{
}
//-----------------------------------------------------------------------------
void DialogConvert2::on_comboBoxPresets_currentIndexChanged(int index)
{
    QString name = this->ui->comboBoxPresets->itemText(index);
    this->presetLoad(name);

    this->updatePreview();
}
//-----------------------------------------------------------------------------
