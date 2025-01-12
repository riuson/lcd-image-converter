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

#include "setuptabtemplates.h"

#include "ui_setuptabtemplates.h"

#include <QFileDialog>

#include "filedialogoptions.h"
#include "preset.h"
#include "templateoptions.h"

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Templates
{

SetupTabTemplates::SetupTabTemplates(Settings::Presets::Preset* preset, QWidget* parent)
    : QWidget(parent), ui(new Ui::SetupTabTemplates)
{
  ui->setupUi(this);
  this->mPreset = preset;

  this->matrixChanged();
}

SetupTabTemplates::~SetupTabTemplates() { delete ui; }

void SetupTabTemplates::matrixChanged()
{
  this->ui->lineEditImage->setText(this->mPreset->templates()->image());
  this->ui->lineEditFont->setText(this->mPreset->templates()->font());
}

void SetupTabTemplates::on_pushButtonBrowseImage_clicked()
{
  QFileDialog dialog(this);
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(Settings::FileDialogOptions::directory(Settings::FileDialogOptions::Dialogs::TemplateImage));
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("All files (*.*)"));
  dialog.setWindowTitle(tr("Open image template file"));

  if (dialog.exec() == QDialog::Accepted) {
    Settings::FileDialogOptions::setDirectory(Settings::FileDialogOptions::Dialogs::TemplateImage,
                                              dialog.directory().absolutePath());
    this->ui->lineEditImage->setText(dialog.selectedFiles().at(0));
    this->mPreset->templates()->setImage(dialog.selectedFiles().at(0));
  }
}

void SetupTabTemplates::on_pushButtonBrowseFont_clicked()
{
  QFileDialog dialog(this);
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(Settings::FileDialogOptions::directory(Settings::FileDialogOptions::Dialogs::TemplateFont));
  dialog.setFileMode(QFileDialog::ExistingFile);
  dialog.setNameFilter(tr("Any file (*.*)"));
  dialog.setWindowTitle(tr("Open font template file"));

  if (dialog.exec() == QDialog::Accepted) {
    Settings::FileDialogOptions::setDirectory(Settings::FileDialogOptions::Dialogs::TemplateFont,
                                              dialog.directory().absolutePath());
    this->ui->lineEditFont->setText(dialog.selectedFiles().at(0));
    this->mPreset->templates()->setFont(dialog.selectedFiles().at(0));
  }
}

void SetupTabTemplates::on_lineEditImage_editingFinished()
{
  const QString value = this->ui->lineEditImage->text();

  if (!value.isEmpty()) {
    this->mPreset->templates()->setImage(value);
  } else {
    this->mPreset->templates()->setImage(":/templates/image_convert");
  }
}

void SetupTabTemplates::on_lineEditFont_editingFinished()
{
  const QString value = this->ui->lineEditFont->text();

  if (!value.isEmpty()) {
    this->mPreset->templates()->setFont(value);
  } else {
    this->mPreset->templates()->setFont(":/templates/font_convert");
  }
}

} // namespace Templates
} // namespace Parts
} // namespace Setup
} // namespace AppUI
