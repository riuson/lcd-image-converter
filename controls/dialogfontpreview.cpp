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

#include "dialogfontpreview.h"
#include "ui_dialogfontpreview.h"
//-----------------------------------------------------------------------------
#include <QPainter>
#include "bitmaphelper.h"
#include "fonthelper.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
DialogFontPreview::DialogFontPreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontPreview)
{
    ui->setupUi(this);
    this->mOriginalImage = QImage(QImage(":/images/template").scaled(20, 10));

    this->on_spinBoxScale_valueChanged(this->ui->spinBoxScale->value());
}
//-----------------------------------------------------------------------------
DialogFontPreview::~DialogFontPreview()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogFontPreview::setDocument(IDocument *document)
{
    this->mDocument = document;
}
//-----------------------------------------------------------------------------
void DialogFontPreview::on_lineEditText_textChanged(const QString &text)
{
    this->mOriginalImage = FontHelper::drawString(this->mDocument->dataContainer(), text);// previewPixmap.toImage();

    // update preview
    this->on_spinBoxScale_valueChanged(this->ui->spinBoxScale->value());
}
//-----------------------------------------------------------------------------
void DialogFontPreview::on_spinBoxScale_valueChanged(int i)
{
    QImage scaled = BitmapHelper::createImageScaled(&this->mOriginalImage, i);
    this->mScaledPixmap = QPixmap::fromImage(scaled);
    this->ui->labelPreview->setPixmap(this->mScaledPixmap);
}
//-----------------------------------------------------------------------------
