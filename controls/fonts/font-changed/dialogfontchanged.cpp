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

#include "dialogfontchanged.h"
#include "ui_dialogfontchanged.h"

DialogFontChanged::DialogFontChanged(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontChanged)
{
    ui->setupUi(this);

    this->mRegenerateAll = false;
}

DialogFontChanged::~DialogFontChanged()
{
    delete ui;
}

bool DialogFontChanged::regenerateAll() const
{
    return this->mRegenerateAll;
}

void DialogFontChanged::on_buttonRegenerateAll_clicked()
{
    this->mRegenerateAll = true;
    //this->setResult(QDialog::Accepted);
    //this->close();
    this->accept();
}

void DialogFontChanged::on_buttonGenerateNewOnly_clicked()
{
    this->mRegenerateAll = false;
    //this->setResult(QDialog::Accepted);
    //this->close();
    this->accept();
}

void DialogFontChanged::on_buttonCancel_clicked()
{
    //this->setResult(QDialog::Rejected);
    //this->close();
    this->reject();
}

