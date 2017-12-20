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
#include "setuptabpreparepreprocessing.h"
#include "setuptabpreparescanning.h"

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Prepare
{

SetupTabPrepare::SetupTabPrepare(Settings::Presets::Preset *preset, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SetupTabPrepare)
{
  ui->setupUi(this);
  this->mPreset = preset;

  this->mTabScanning = new SetupTabPrepareScanning(preset, this);
  this->ui->tabWidgetPrepare->addTab(this->mTabScanning, tr("Scanning"));

  this->mTabPreprocessing = new SetupTabPreparePreprocessing(preset, this);
  this->ui->tabWidgetPrepare->addTab(this->mTabPreprocessing, tr("Preprocessing"));

  this->matrixChanged();
}

SetupTabPrepare::~SetupTabPrepare()
{
  delete ui;
}

void SetupTabPrepare::matrixChanged()
{
  this->mTabScanning->matrixChanged();
  this->mTabPreprocessing->matrixChanged();
}

} // namespace Prepare
} // namespace Parts
} // namespace Setup
} // namespace AppUI
