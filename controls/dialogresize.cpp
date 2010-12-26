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

#include "dialogresize.h"
#include "ui_dialogresize.h"
//-----------------------------------------------------------------------------
DialogResize::DialogResize(int width, int height, int offsetX, int offsetY, bool center, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResize)
{
    ui->setupUi(this);
    this->mWidth = width;
    this->mHeight = height;
    this->mOffsetX = offsetX;
    this->mOffsetY = offsetY;
    this->mCenter = center;

    this->ui->spinBoxWidth->setValue(width);
    this->ui->spinBoxHeight->setValue(height);
    this->ui->spinBoxOffsetX->setValue(offsetX);
    this->ui->spinBoxOffsetY->setValue(offsetY);
    this->ui->checkBoxCenter->setChecked(center);
}
//-----------------------------------------------------------------------------
DialogResize::~DialogResize()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogResize::getResizeInfo(int *width, int *height, int *offsetX, int *offsetY, bool *center)
{
    *width = this->mWidth;
    *height = this->mHeight;
    *offsetX = this->mOffsetX;
    *offsetY = this->mOffsetY;
    *center = this->mCenter;
}
//-----------------------------------------------------------------------------
void DialogResize::on_checkBoxCenter_stateChanged(int state)
{
    bool manualOffset = (state == Qt::Unchecked);
    this->ui->labelOffsetX->setEnabled(manualOffset);
    this->ui->labelOffsetY->setEnabled(manualOffset);
    this->ui->spinBoxOffsetX->setEnabled(manualOffset);
    this->ui->spinBoxOffsetY->setEnabled(manualOffset);
    this->mCenter = !manualOffset;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxWidth_valueChanged(int value)
{
    this->mWidth = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxHeight_valueChanged(int value)
{
    this->mHeight = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxOffsetX_valueChanged(int value)
{
    this->mOffsetX = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxOffsetY_valueChanged(int value)
{
    this->mOffsetY = value;
}
//-----------------------------------------------------------------------------
