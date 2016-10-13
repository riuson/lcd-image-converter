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

#include "dialogexternaleditor.h"
#include "ui_dialogexternaleditor.h"

#include <QFileDialog>
#include "externaltooloptions.h"

DialogExternalEditor::DialogExternalEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExternalEditor)
{
    ui->setupUi(this);

    this->ui->lineEdit->setText(ExternalToolOptions::imageEditor());
}

DialogExternalEditor::~DialogExternalEditor()
{
    delete ui;
}

void DialogExternalEditor::done(int result)
{
    if (result == QDialog::Accepted)
    {
        ExternalToolOptions::setImageEditor(this->ui->lineEdit->text());
    }
    QDialog::done(result);
}

void DialogExternalEditor::on_pushButtonBrowse_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.selectFile(this->ui->lineEdit->text());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("All files (*.*)"));
    dialog.setWindowTitle(tr("Select application"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEdit->setText(dialog.selectedFiles().at(0));
    }
}

