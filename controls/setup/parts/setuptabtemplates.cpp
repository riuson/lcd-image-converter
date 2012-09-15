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

#include "setuptabtemplates.h"
#include "ui_setuptabtemplates.h"
//-----------------------------------------------------------------------------
#include <QFileDialog>
#include "preset.h"
#include "templateoptions.h"
//-----------------------------------------------------------------------------
SetupTabTemplates::SetupTabTemplates(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabTemplates)
{
    ui->setupUi(this);
    this->mPreset = preset;

    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabTemplates::~SetupTabTemplates()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::matrixChanged()
{
    this->ui->lineEditImage->setText(this->mPreset->templates()->image());
    this->ui->lineEditFont->setText(this->mPreset->templates()->font());
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::on_pushButtonBrowseImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open image template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mPreset->templates()->setImage(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::on_pushButtonBrowseFont_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open font template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mPreset->templates()->setFont(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
