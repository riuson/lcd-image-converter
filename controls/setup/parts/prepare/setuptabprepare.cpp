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

#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
#include "bitmaphelper.h"
#include "converterhelper.h"
#include "demogenerator.h"
#include "setupdialogoptions.h"

SetupTabPrepare::SetupTabPrepare(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabPrepare)
{
    ui->setupUi(this);
    this->mPreset = preset;
    this->mPixmapScanning = QPixmap();
    this->mPixmapScanPreview = QPixmap();
    this->mDemoGen = new DemoGenerator(this->mPreset, this);
    this->connect(this->mDemoGen, SIGNAL(pixmapChanged(const QPixmap&)), SLOT(demoPixmapChanged(const QPixmap&)));
    this->connect(this->mDemoGen, SIGNAL(errorHandled(QString)), SLOT(demoScriptError(QString)));

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(QString("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(QString("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(QString("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxScanMain->addItem(tr("Top to Bottom"), QVariant(TopToBottom));
    this->ui->comboBoxScanMain->addItem(tr("Bottom to Top"), QVariant(BottomToTop));
    this->ui->comboBoxScanMain->addItem(tr("Left to Right"), QVariant(LeftToRight));
    this->ui->comboBoxScanMain->addItem(tr("Right to Left"), QVariant(RightToLeft));

    this->ui->comboBoxScanSub->addItem(tr("Forward"), QVariant(Forward));
    this->ui->comboBoxScanSub->addItem(tr("Backward"), QVariant(Backward));

    this->ui->spinBoxAnimationTime->setValue(SetupDialogOptions::animationTotalTime());
    this->ui->spinBoxAnimationInterval->setValue(SetupDialogOptions::animationInterval());

    this->matrixChanged();
}

SetupTabPrepare::~SetupTabPrepare()
{
    delete this->mDemoGen;
    delete ui;
}

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

    this->updateState();

    this->updateScript();
}

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

void SetupTabPrepare::on_checkBoxInverse_toggled(bool value)
{
    this->mPreset->prepare()->setInverse(value);
}

void SetupTabPrepare::on_horizontalScrollBarEdge_valueChanged(int value)
{
    this->mPreset->prepare()->setEdge(value);
}

void SetupTabPrepare::on_checkBoxBands_toggled(bool value)
{
    this->mPreset->prepare()->setBandScanning(value);
    this->updateState();
}

void SetupTabPrepare::on_spinBoxBandWidth_valueChanged(int value)
{
    this->mPreset->prepare()->setBandWidth(value);
}

void SetupTabPrepare::on_checkBoxUseCustomScript_toggled(bool value)
{
    this->mPreset->prepare()->setUseCustomScript(value);
    this->updateState();
}

void SetupTabPrepare::on_plainTextEditCustomScript_textChanged()
{
    if (this->mPreset->prepare()->useCustomScript())
    {
        QString str = this->ui->plainTextEditCustomScript->toPlainText();
        this->mPreset->prepare()->setCustomScript(str);
    }
}

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
    this->ui->plainTextEditCustomScript->setReadOnly(!this->mPreset->prepare()->useCustomScript());
}

void SetupTabPrepare::updateScript()
{
    QString script = ConverterHelper::scanScript(this->mPreset);
    if (this->ui->plainTextEditCustomScript->toPlainText() != script)
        this->ui->plainTextEditCustomScript->setPlainText(script);

    this->mDemoGen->setScript(script);
}

void SetupTabPrepare::demoPixmapChanged(const QPixmap &pixmap)
{
    this->mPixmapScanPreview = pixmap;

    this->ui->labelScanPreview->setPixmap(this->mPixmapScanPreview);
    this->ui->spinBoxAnimationTime->setEnabled(true);
    this->ui->spinBoxAnimationInterval->setEnabled(true);

    this->ui->labelErrorMessage->hide();
}

void SetupTabPrepare::demoScriptError(const QString &value)
{
    this->ui->spinBoxAnimationTime->setEnabled(false);
    this->ui->spinBoxAnimationInterval->setEnabled(false);

    this->ui->labelErrorMessage->setText(value);
    this->ui->labelErrorMessage->show();
}

void SetupTabPrepare::on_spinBoxAnimationTime_valueChanged(int value)
{
    this->mDemoGen->setAnimationTime(value);
    SetupDialogOptions::setAnimationTime(value);
}

void SetupTabPrepare::on_spinBoxAnimationInterval_valueChanged(int value)
{
    this->mDemoGen->setAnimationInterval(value);
    SetupDialogOptions::setAnimationInterval(value);
}

