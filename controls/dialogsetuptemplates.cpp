/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#include "dialogsetuptemplates.h"
#include "ui_dialogsetuptemplates.h"

#include <QFileDialog>
#include <QSettings>
//-----------------------------------------------------------------------------
DialogSetupTemplates::DialogSetupTemplates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupTemplates)
{
    ui->setupUi(this);

    QSettings sett;
    sett.beginGroup("setup");

    this->ui->lineEditImage->setText(sett.value("templateImage", ":/templates/image_convert").toString());
    this->ui->lineEditFont->setText(sett.value("templateFont", ":/templates/font_convert").toString());

    sett.endGroup();
}
//-----------------------------------------------------------------------------
DialogSetupTemplates::~DialogSetupTemplates()
{
    QSettings sett;
    sett.beginGroup("setup");

    sett.setValue("templateImage", this->ui->lineEditImage->text());
    sett.setValue("templateFont", this->ui->lineEditFont->text());

    sett.endGroup();

    delete ui;
}
//-----------------------------------------------------------------------------
void DialogSetupTemplates::on_pushButtonBrowseImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open image template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEditImage->setText(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
void DialogSetupTemplates::on_pushButtonBrowseFont_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open font template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEditFont->setText(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
