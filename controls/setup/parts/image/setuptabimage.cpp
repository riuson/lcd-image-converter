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

#include "setuptabimage.h"
#include "ui_setuptabimage.h"

#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"

SetupTabImage::SetupTabImage(Settings::Presets::Preset *preset, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SetupTabImage)
{
  ui->setupUi(this);
  this->mPreset = preset;

  this->ui->comboBoxBlockSize->addItem(tr("8 bit"),  static_cast<int>(Parsing::Conversion::Options::DataBlockSize::Data8));
  this->ui->comboBoxBlockSize->addItem(tr("16 bit"), static_cast<int>(Parsing::Conversion::Options::DataBlockSize::Data16));
  this->ui->comboBoxBlockSize->addItem(tr("24 bit"), static_cast<int>(Parsing::Conversion::Options::DataBlockSize::Data24));
  this->ui->comboBoxBlockSize->addItem(tr("32 bit"), static_cast<int>(Parsing::Conversion::Options::DataBlockSize::Data32));

  this->matrixChanged();
}

SetupTabImage::~SetupTabImage()
{
  delete ui;
}

void SetupTabImage::matrixChanged()
{
  this->ui->checkBoxSplitToRows->setChecked(this->mPreset->image()->splitToRows());

  if (this->mPreset->image()->bytesOrder() == Parsing::Conversion::Options::BytesOrder::LittleEndian) {
    this->ui->radioButtonLittleEndian->setChecked(true);
  } else {
    this->ui->radioButtonBigEndian->setChecked(true);
  }

  int index = this->ui->comboBoxBlockSize->findData(static_cast<int>(this->mPreset->image()->blockSize()));

  if (index >= 0) {
    this->ui->comboBoxBlockSize->setCurrentIndex(index);
  }

  if (this->mPreset->image()->bytesOrder() == Parsing::Conversion::Options::BytesOrder::LittleEndian) {
    this->ui->radioButtonLittleEndian->setChecked(true);
  } else {
    this->ui->radioButtonBigEndian->setChecked(true);
  }

  this->ui->checkBoxCompressionRle->setChecked(this->mPreset->image()->compressionRle());
  this->ui->spinBoxRleMinLength->setValue(this->mPreset->image()->compressionRleMinLength());

  this->ui->checkBoxBlockDefaultOnes->setChecked(this->mPreset->image()->blockDefaultOnes());

  this->ui->lineEditBlockPrefix->setText(this->mPreset->image()->blockPrefix());
  this->ui->lineEditBlockSuffix->setText(this->mPreset->image()->blockSuffix());
  this->ui->lineEditBlockDelimiter->setText(this->mPreset->image()->blockDelimiter());

  this->ui->lineEditPreviewPrefix->setText(this->mPreset->image()->previewPrefix());
  this->ui->lineEditPreviewSuffix->setText(this->mPreset->image()->previewSuffix());
  this->ui->lineEditPreviewDelimiter->setText(this->mPreset->image()->previewDelimiter());
  this->ui->textEditPreviewLevels->setPlainText(this->mPreset->image()->previewLevels());
}

void SetupTabImage::on_checkBoxSplitToRows_toggled(bool value)
{
  this->mPreset->image()->setSplitToRows(value);
}

void SetupTabImage::on_radioButtonLittleEndian_toggled(bool value)
{
  if (value) {
    this->mPreset->image()->setBytesOrder(Parsing::Conversion::Options::BytesOrder::LittleEndian);
  } else {
    this->mPreset->image()->setBytesOrder(Parsing::Conversion::Options::BytesOrder::BigEndian);
  }
}

void SetupTabImage::on_comboBoxBlockSize_currentIndexChanged(int index)
{
  QVariant data = this->ui->comboBoxBlockSize->itemData(index);
  bool ok;
  int a = data.toInt(&ok);

  if (ok) {
    this->mPreset->image()->setBlockSize((Settings::Presets::DataBlockSize)a);
  }
}

void SetupTabImage::on_checkBoxCompressionRle_toggled(bool value)
{
  this->mPreset->image()->setCompressionRle(value);
}

void SetupTabImage::on_spinBoxRleMinLength_valueChanged(int value)
{
  this->mPreset->image()->setCompressionRleMinLength((quint32)value);
}

void SetupTabImage::on_checkBoxBlockDefaultOnes_toggled(bool value)
{
  this->mPreset->image()->setBlockDefaultOnes(value);
}

void SetupTabImage::on_lineEditBlockPrefix_textEdited(const QString &value)
{
  this->mPreset->image()->setBlockPrefix(value);
}

void SetupTabImage::on_lineEditBlockSuffix_textEdited(const QString &value)
{
  this->mPreset->image()->setBlockSuffix(value);
}

void SetupTabImage::on_lineEditBlockDelimiter_textEdited(const QString &value)
{
  this->mPreset->image()->setBlockDelimiter(value);
}

void SetupTabImage::on_lineEditPreviewPrefix_textEdited(const QString &value)
{
  this->mPreset->image()->setPreviewPrefix(value);
}

void SetupTabImage::on_lineEditPreviewSuffix_textEdited(const QString &value)
{
  this->mPreset->image()->setPreviewSuffix(value);
}

void SetupTabImage::on_lineEditPreviewDelimiter_textEdited(const QString &value)
{
  this->mPreset->image()->setPreviewDelimiter(value);
}

void SetupTabImage::on_textEditPreviewLevels_textChanged()
{
  this->mPreset->image()->setPreviewLevels(this->ui->textEditPreviewLevels->toPlainText());
}
