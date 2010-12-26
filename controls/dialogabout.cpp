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

#include "dialogabout.h"
#include "ui_dialogabout.h"

#include <QTextStream>
#include <QFile>
//-----------------------------------------------------------------------------
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    QFile file(":/text/gpl3");
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString license = stream.readAll();
        file.close();

        this->ui->textEdit->setText(license);
    }
}
//-----------------------------------------------------------------------------
DialogAbout::~DialogAbout()
{
    delete ui;
}
//-----------------------------------------------------------------------------
