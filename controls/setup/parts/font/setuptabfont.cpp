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

#include "setuptabfont.h"
#include "ui_setuptabfont.h"

#include <QCompleter>
#include "preset.h"
#include "fontoptions.h"

SetupTabFont::SetupTabFont(Preset *preset, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SetupTabFont)
{
  ui->setupUi(this);
  this->mPreset = preset;

  QString encoding = this->mPreset->font()->encoding();

  this->ui->comboBoxEncoding->addItems(FontOptions::encodings());

  int index = this->ui->comboBoxEncoding->findData(encoding);

  if (index >= 0) {
    this->ui->comboBoxEncoding->setCurrentIndex(index);
  }

  CharactersSortOrder order = this->mPreset->font()->sortOrder();

  this->ui->comboBoxSorting->addItem(this->sortingName(CharactersSortNone), CharactersSortNone);
  this->ui->comboBoxSorting->addItem(this->sortingName(CharactersSortAscending), CharactersSortAscending);
  this->ui->comboBoxSorting->addItem(this->sortingName(CharactersSortDescending), CharactersSortDescending);
  index = this->ui->comboBoxSorting->findData(order);

  if (index >= 0) {
    this->ui->comboBoxSorting->setCurrentIndex(index);
  }

  this->matrixChanged();

  this->mEncodingCompleter = new QCompleter(FontOptions::encodings(), this);
  this->mEncodingCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  this->mEncodingCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  this->mEncodingCompleter->setFilterMode(Qt::MatchContains);
  this->ui->comboBoxEncoding->setCompleter(this->mEncodingCompleter);
}

SetupTabFont::~SetupTabFont()
{
  delete ui;
}

void SetupTabFont::matrixChanged()
{
  int index = this->ui->comboBoxEncoding->findText(this->mPreset->font()->encoding());

  if (index >= 0) {
    this->ui->comboBoxEncoding->setCurrentIndex(index);
  }

  index = this->ui->comboBoxSorting->findData(this->mPreset->font()->sortOrder());

  if (index >= 0) {
    this->ui->comboBoxSorting->setCurrentIndex(index);
  }

  this->ui->checkBoxBom->setChecked(this->mPreset->font()->bom());
}

const QString SetupTabFont::sortingName(Parsing::Conversion::Options::CharactersSortOrder value) const
{
  QString result;

  switch (value) {
    case Parsing::Conversion::Options::CharactersSortNone:
      result = tr("None");
      break;

    case Parsing::Conversion::Options::CharactersSortAscending:
      result = tr("Ascending");
      break;

    case Parsing::Conversion::Options::CharactersSortDescending:
      result = tr("Descending");
      break;

    default:
      result = QString();
      break;
  }

  return result;
}

void SetupTabFont::on_checkBoxBom_toggled(bool value)
{
  this->mPreset->font()->setBom(value);
}

void SetupTabFont::on_comboBoxEncoding_currentIndexChanged(const QString &value)
{
  this->mPreset->font()->setEncoding(value);
}

void SetupTabFont::on_comboBoxSorting_currentIndexChanged(int index)
{
  bool ok = false;
  CharactersSortOrder order = (CharactersSortOrder)this->ui->comboBoxSorting->itemData(index).toInt(&ok);

  if (ok) {
    this->mPreset->font()->setSortOrder(order);
  }
}

