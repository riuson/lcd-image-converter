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

#include "setuptabprepare.h"
#include "ui_setuptabprepare.h"
//-----------------------------------------------------------------------------
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "bitmaphelper.h"
#include "converterhelper.h"
//-----------------------------------------------------------------------------
SetupTabPrepare::SetupTabPrepare(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabPrepare)
{
    ui->setupUi(this);
    this->mPreset = preset;
    this->mPixmapScanning = QPixmap();

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(tr("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxScanMain->addItem(tr("Top to Bottom"), QVariant(TopToBottom));
    this->ui->comboBoxScanMain->addItem(tr("Bottom to Top"), QVariant(BottomToTop));
    this->ui->comboBoxScanMain->addItem(tr("Left to Right"), QVariant(LeftToRight));
    this->ui->comboBoxScanMain->addItem(tr("Right to Left"), QVariant(RightToLeft));

    this->ui->comboBoxScanSub->addItem(tr("Forward"), QVariant(Forward));
    this->ui->comboBoxScanSub->addItem(tr("Backward"), QVariant(Backward));

    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabPrepare::~SetupTabPrepare()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::matrixChanged()
{
    int index = this->ui->comboBoxConversionType->findData(this->mPreset->prepare()->convType());
    if (index >= 0)
        this->ui->comboBoxConversionType->setCurrentIndex(index);

    index = this->ui->comboBoxMonochromeType->findData(this->mPreset->prepare()->monoType());
    if (index >= 0)
        this->ui->comboBoxMonochromeType->setCurrentIndex(index);

    index = this->ui->comboBoxScanMain->findData(this->mPreset->prepare()->scanMain());
    if (index >= 0)
        this->ui->comboBoxScanMain->setCurrentIndex(index);

    index = this->ui->comboBoxScanSub->findData(this->mPreset->prepare()->scanSub());
    if (index >= 0)
        this->ui->comboBoxScanSub->setCurrentIndex(index);

    this->ui->horizontalScrollBarEdge->setValue(this->mPreset->prepare()->edge());

    this->ui->checkBoxBands->setChecked(this->mPreset->prepare()->bandScanning());

    this->ui->spinBoxBandWidth->setValue(this->mPreset->prepare()->bandWidth());

    this->ui->checkBoxInverse->setChecked(this->mPreset->prepare()->inverse());

    this->ui->checkBoxUseCustomScript->setChecked(this->mPreset->prepare()->useCustomScript());

    if (this->mPreset->prepare()->useCustomScript())
    {
        if (this->ui->plainTextEditCustomScript->toPlainText() != this->mPreset->prepare()->customScript())
            this->ui->plainTextEditCustomScript->setPlainText(this->mPreset->prepare()->customScript());
    }
    else
    {
        QString script = ConverterHelper::scanScript(this->mPreset);
        this->ui->plainTextEditCustomScript->setPlainText(script);
    }

    this->updateScanningPreview();

    this->updateState();
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::updateScanningPreview()
{
    Rotate rotate = RotateNone;
    bool flipHorizontal = false;
    bool flipVertical = false;

    this->modificationsFromScan(&rotate, &flipHorizontal, &flipVertical);

    QImage image(":/demos/scanning");

    // load another image for bands
    if (this->mPreset->prepare()->bandScanning())
        image.load(":/demos/scanning_band");

    switch (rotate)
    {
    case Rotate90:
        image = BitmapHelper::rotate90(&image);
        break;
    case Rotate180:
        image = BitmapHelper::rotate180(&image);
        break;
    case Rotate270:
        image = BitmapHelper::rotate270(&image);
        break;
    default:
        image = image;
        break;

    }

    if (flipHorizontal)
        image = BitmapHelper::flipHorizontal(&image);

    if (flipVertical)
        image = BitmapHelper::flipVertical(&image);

    if (this->mPreset->prepare()->inverse())
        image.invertPixels();

    this->mPixmapScanning = QPixmap::fromImage(image);

    this->ui->labelScanningOrder->setPixmap(this->mPixmapScanning);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::modificationsFromScan(
        Rotate *rotate,
        bool *flipHorizontal,
        bool *flipVertical) const
{
    bool forward = (this->mPreset->prepare()->scanSub() == Forward);

    switch (this->mPreset->prepare()->scanMain())
    {
    case TopToBottom:
    {
        if (forward)
        {
            *rotate = RotateNone;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        else
        {
            *rotate = RotateNone;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        break;
    }
    case BottomToTop:
    {
        if (forward)
        {
            *rotate = RotateNone;
            *flipHorizontal = false;
            *flipVertical = true;
        }
        else
        {
            *rotate = Rotate180;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        break;
    }
    case LeftToRight:
    {
        if (forward)
        {
            *rotate = Rotate90;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        else
        {
            *rotate = Rotate270;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        break;
    }
    case RightToLeft:
    {
        if (forward)
        {
            *rotate = Rotate90;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        else
        {
            *rotate = Rotate270;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        break;
    }
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxConversionType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxConversionType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mPreset->prepare()->setConvType((ConversionType)a);
        this->updateState();
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxMonochromeType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxMonochromeType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mPreset->prepare()->setMonoType((MonochromeType)a);
        this->updateState();
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxScanMain_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxScanMain->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ScanMainDirection dir = (ScanMainDirection)a;
        this->mPreset->prepare()->setScanMain(dir);
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxScanSub_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxScanSub->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        ScanSubDirection dir = (ScanSubDirection)a;
        this->mPreset->prepare()->setScanSub(dir);
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxInverse_toggled(bool value)
{
    this->mPreset->prepare()->setInverse(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_horizontalScrollBarEdge_valueChanged(int value)
{
    this->mPreset->prepare()->setEdge(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxBands_toggled(bool value)
{
    this->mPreset->prepare()->setBandScanning(value);
    this->updateState();
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_spinBoxBandWidth_valueChanged(int value)
{
    this->mPreset->prepare()->setBandWidth(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxUseCustomScript_toggled(bool value)
{
    this->mPreset->prepare()->setUseCustomScript(value);
    this->updateState();
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_plainTextEditCustomScript_textChanged()
{
    QString str = this->ui->plainTextEditCustomScript->toPlainText();
    this->mPreset->prepare()->setCustomScript(str);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::updateState()
{
    // conversion type
    if (this->mPreset->prepare()->convType() == ConversionTypeMonochrome)
    {
        this->ui->comboBoxMonochromeType->setEnabled(true);
        if (this->mPreset->prepare()->monoType() == MonochromeTypeEdge)
            this->ui->horizontalScrollBarEdge->setEnabled(true);
        else
            this->ui->horizontalScrollBarEdge->setEnabled(false);
    }
    else
    {
        this->ui->comboBoxMonochromeType->setEnabled(false);
        this->ui->horizontalScrollBarEdge->setEnabled(false);
    }

    // monochrome type
    if (this->mPreset->prepare()->convType() == ConversionTypeMonochrome)
    {
        this->ui->comboBoxMonochromeType->setEnabled(true);
        if (this->mPreset->prepare()->monoType() == MonochromeTypeEdge)
            this->ui->horizontalScrollBarEdge->setEnabled(true);
        else
            this->ui->horizontalScrollBarEdge->setEnabled(false);
    }
    else
    {
        this->ui->comboBoxMonochromeType->setEnabled(false);
        this->ui->horizontalScrollBarEdge->setEnabled(false);
    }

    // band scan
    this->ui->spinBoxBandWidth->setEnabled(this->mPreset->prepare()->bandScanning());

    // use custom script
    this->ui->plainTextEditCustomScript->setEnabled(this->mPreset->prepare()->useCustomScript());
}
//-----------------------------------------------------------------------------
