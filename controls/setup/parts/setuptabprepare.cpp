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

#include "setuptabprepare.h"
#include "ui_setuptabprepare.h"
//-----------------------------------------------------------------------------
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
SetupTabPrepare::SetupTabPrepare(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabPrepare)
{
    ui->setupUi(this);
    this->mPreset = preset;

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(tr("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxRotate->addItem(tr("None"), QVariant(RotateNone));
    this->ui->comboBoxRotate->addItem(tr("90 Clockwise"), QVariant(Rotate90));// \u00b0
    this->ui->comboBoxRotate->addItem(tr("180"), QVariant(Rotate180));
    this->ui->comboBoxRotate->addItem(tr("90 Counter-Clockwise"), QVariant(Rotate270));

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

    index = this->ui->comboBoxRotate->findData(this->mPreset->prepare()->rotate());
    if (index >= 0)
        this->ui->comboBoxRotate->setCurrentIndex(index);

    this->ui->horizontalScrollBarEdge->setValue(this->mPreset->prepare()->edge());

    this->ui->checkBoxFlipHorizontal->setChecked(this->mPreset->prepare()->flipHorizontal());
    this->ui->checkBoxFlipVertical->setChecked(this->mPreset->prepare()->flipVertical());
    this->ui->checkBoxInverse->setChecked(this->mPreset->prepare()->inverse());
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
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxRotate_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxRotate->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        Rotate rotate = (Rotate)a;
        this->mPreset->prepare()->setRotate(rotate);
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxFlipHorizontal_toggled(bool value)
{
    this->mPreset->prepare()->setFlipHorizontal(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxFlipVertical_toggled(bool value)
{
    this->mPreset->prepare()->setFlipVertical(value);
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
