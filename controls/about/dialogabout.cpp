/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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
#include "revisioninfo.h"
#include "bitmaphelper.h"

namespace AppUI
{

DialogAbout::DialogAbout(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogAbout)
{
  ui->setupUi(this);

  // hide ? button from title
  this->setWindowFlags(this->windowFlags() & (~Qt::WindowContextHelpButtonHint));

  // update icon
  QIcon icon;
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 16)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 24)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 32)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 48)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 64)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 128)));
  icon.addPixmap(QPixmap::fromImage(Parsing::Conversion::BitmapHelper::fromSvg(QString(":/images/lic_icon"), 256)));
  this->setWindowIcon(icon);

  this->showLicense();

  // show revision info
  {
    QString about = this->ui->labelInfo->text();
    QString formattedAbout = QString(about).arg(
                               VersionControl::RevisionInfo::hash(),
                               VersionControl::RevisionInfo::hash_abbr(),
                               VersionControl::RevisionInfo::date());
    this->ui->labelInfo->setText(formattedAbout);
  }

  // show Qt version
  {
    QString about = this->ui->labelInfo->text();
    QString formattedAbout = QString(about).arg(qVersion());
    this->ui->labelInfo->setText(formattedAbout);
  }

  // focus on Close button
  this->ui->buttonBox->setFocus();
}

DialogAbout::~DialogAbout()
{
  delete ui;
}

void DialogAbout::showLicense()
{
  QFile file_license(":/text/gpl3");

  if (file_license.open(QIODevice::ReadOnly)) {
    QTextStream stream(&file_license);
    QString license = stream.readAll();
    file_license.close();

    this->ui->textBrowser->setText(license);
  }
}

} // namespace AppUI
