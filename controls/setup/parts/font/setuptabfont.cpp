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

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Font
{

SetupTabFont::SetupTabFont(Settings::Presets::Preset *preset, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SetupTabFont)
{
  ui->setupUi(this);
  this->mPreset = preset;

  QString encoding = this->mPreset->font()->encoding();

  this->ui->comboBoxEncoding->addItems(Settings::Presets::FontOptions::encodings());

  int index = this->ui->comboBoxEncoding->findData(encoding);

  if (index >= 0) {
    this->ui->comboBoxEncoding->setCurrentIndex(index);
  }

  Settings::Presets::CharactersSortOrder order = this->mPreset->font()->sortOrder();

  this->ui->comboBoxSorting->addItem(
    this->sortingName(Parsing::Conversion::Options::CharactersSortOrder::None),
    static_cast<int>(Parsing::Conversion::Options::CharactersSortOrder::None));
  this->ui->comboBoxSorting->addItem(
    this->sortingName(Parsing::Conversion::Options::CharactersSortOrder::Ascending),
    static_cast<int>(Parsing::Conversion::Options::CharactersSortOrder::Ascending));
  this->ui->comboBoxSorting->addItem(
    this->sortingName(Parsing::Conversion::Options::CharactersSortOrder::Descending),
    static_cast<int>(Parsing::Conversion::Options::CharactersSortOrder::Descending));
  index = this->ui->comboBoxSorting->findData(static_cast<int>(order));

  if (index >= 0) {
    this->ui->comboBoxSorting->setCurrentIndex(index);
  }

  this->matrixChanged();

  this->mEncodingCompleter = new QCompleter(Settings::Presets::FontOptions::encodings(), this);
  this->mEncodingCompleter->setCaseSensitivity(Qt::CaseInsensitive);
  this->mEncodingCompleter->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  this->mEncodingCompleter->setFilterMode(Qt::MatchContains);
  this->ui->comboBoxEncoding->setCompleter(this->mEncodingCompleter);

  this->ui->lineEditEscapedCharacters->setText(this->mPreset->font()->escapedCharacters());
  this->ui->lineEditEscapePrefix->setText(this->mPreset->font()->escapePrefix());
  this->ui->lineEditEscapeSuffix->setText(this->mPreset->font()->escapeSuffix());
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

  index = this->ui->comboBoxSorting->findData(static_cast<int>(this->mPreset->font()->sortOrder()));

  if (index >= 0) {
    this->ui->comboBoxSorting->setCurrentIndex(index);
  }

  this->ui->checkBoxBom->setChecked(this->mPreset->font()->bom());
  this->ui->checkBoxSkipMissingCharacters->setChecked(this->mPreset->font()->skipMissingCharacters());

  this->ui->lineEditEscapedCharacters->setText(this->mPreset->font()->escapedCharacters());
  this->ui->lineEditEscapePrefix->setText(this->mPreset->font()->escapePrefix());
  this->ui->lineEditEscapeSuffix->setText(this->mPreset->font()->escapeSuffix());

  this->ui->checkBoxCompactGlyphs->setChecked(this->mPreset->font()->compactGlyphs());
}

const QString SetupTabFont::sortingName(Parsing::Conversion::Options::CharactersSortOrder value) const
{
  QString result;

  switch (value) {
    case Parsing::Conversion::Options::CharactersSortOrder::None:
      result = tr("None");
      break;

    case Parsing::Conversion::Options::CharactersSortOrder::Ascending:
      result = tr("Ascending");
      break;

    case Parsing::Conversion::Options::CharactersSortOrder::Descending:
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
  Settings::Presets::CharactersSortOrder order = (Settings::Presets::CharactersSortOrder)this->ui->comboBoxSorting->itemData(index).toInt(&ok);

  if (ok) {
    this->mPreset->font()->setSortOrder(order);
  }
}

void SetupTabFont::on_checkBoxSkipMissingCharacters_toggled(bool value)
{
  this->mPreset->font()->setSkipMissingCharacters(value);
}

void SetupTabFont::on_lineEditEscapedCharacters_textEdited(const QString &value)
{
  this->mPreset->font()->setEscapedCharacters(value);
}

void SetupTabFont::on_lineEditEscapePrefix_textEdited(const QString &value)
{
  this->mPreset->font()->setEscapePrefix(value);
}

void SetupTabFont::on_lineEditEscapeSuffix_textEdited(const QString &value)
{
  this->mPreset->font()->setEscapeSuffix(value);
}

void SetupTabFont::on_checkBoxCompactGlyphs_toggled(bool value)
{
  this->mPreset->font()->setCompactGlyphs(value);
}

} // namespace Font
} // namespace Font
} // namespace Setup
} // namespace AppUI
