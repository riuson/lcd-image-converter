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

#include "dialogsavechanges.h"
#include "ui_dialogsavechanges.h"
//-----------------------------------------------------------------------------
DialogSaveChanges::DialogSaveChanges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveChanges)
{
    ui->setupUi(this);
    this->mAnswer = Cancel;
}
//-----------------------------------------------------------------------------
DialogSaveChanges::~DialogSaveChanges()
{
    delete ui;
}
//-----------------------------------------------------------------------------
int DialogSaveChanges::answer()
{
    return this->mAnswer;
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonSave_clicked()
{
    this->mAnswer = Save;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonSaveAs_clicked()
{
    this->mAnswer = SaveAs;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonDontSave_clicked()
{
    this->mAnswer = DontSave;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonCancel_clicked()
{
    this->mAnswer = Cancel;
    this->close();
}
//-----------------------------------------------------------------------------
