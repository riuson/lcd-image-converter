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

#include "dialogfontselect.h"
#include "ui_dialogfontselect.h"

#include <QTableWidgetSelectionRange>
#include <QColorDialog>
#include <appsettings.h>
#include "charactersmodel.h"
#include "unicodeblocksmodel.h"
#include "unicodeblocksfiltermodel.h"
#include "dialogfontrange.h"
#include "fonthelper.h"
#include "fonteditoroptions.h"
#include "fontparameters.h"
#include "dialogfontselectdata.h"

namespace AppUI
{
namespace Fonts
{

DialogFontSelect::DialogFontSelect(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogFontSelect)
{
  ui->setupUi(this);

  this->mData = new DialogFontSelectData(this);

  this->mData->charactersModel()->setCodesRange(0x0000, 0x00ff);
  this->ui->tableView->setModel(this->mData->charactersModel());

  QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
  this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectionChanged(QItemSelection, QItemSelection)));

  this->ui->radioButtonMonospaced->setChecked(false);
  this->ui->radioButtonProportional->setChecked(false);
  this->ui->checkBoxAntialiasing->setChecked(false);

  this->ui->listViewBlocks->setModel(this->mData->unicodeBlocksModel());

  selectionModel = this->ui->listViewBlocks->selectionModel();
  this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this->mData, SLOT(setUnicodeRange(QItemSelection, QItemSelection)));

  // Set font
  //this->connectFontFamilyList(false);
  // Get styles
  this->connect(this->mData, SIGNAL(stylesListChanged(QStringList, QString)), SLOT(on_stylesListChanged(QStringList, QString)));
  // Set style
  //this->connectFontStyleList(false);
  // Get sizes
  this->connect(this->mData, SIGNAL(sizesListChanged(QList<int>, int)), SLOT(on_sizesListChanged(QList<int>, int)));
  // Set size
  //this->connectFontSizeList(false);
  // Set unicode blocks filter
  this->connect(this->ui->lineEditUnicodeBlocksFilter, SIGNAL(textChanged(QString)), this->mData, SLOT(setUnicodeBlocksFilter(QString)));

  // Character's line edit
  this->connect(this->ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(on_lineEdit_textChanged(QString)));
  this->connect(this->mData, SIGNAL(charactersListChanged(QString)), SLOT(on_charactersListChanged(QString)));

  // Apply changed font to character's table
  this->connect(this->mData, SIGNAL(fontChanged(QFont)), SLOT(on_fontChanged(QFont)));
  this->connect(this->mData, SIGNAL(fontMeasured(int, QSize, QSize)), SLOT(on_fontMeasured(int, QSize, QSize)));

  // Checkbox/RadioButton
  this->connect(this->ui->radioButtonMonospaced, SIGNAL(toggled(bool)), this->mData, SLOT(setMonospaced(bool)));
  this->connect(this->ui->checkBoxAntialiasing, SIGNAL(toggled(bool)), this->mData, SLOT(setAntialiasing(bool)));
  this->connect(this->mData, SIGNAL(antialiasingChanged(bool)), this->ui->checkBoxAntialiasing, SLOT(setChecked(bool)));
  this->connect(this->mData, SIGNAL(monospacedChanged(bool)), SLOT(on_monospacedChanged(bool)));

  // Colors
  this->connect(this->mData, SIGNAL(colorsChanged(QColor, QColor)), SLOT(updateColorIcons(QColor, QColor)));

  // Sort characters
  this->connect(this->ui->pushButtonSort, SIGNAL(clicked(bool)), this->mData, SLOT(resort()));

  // Multiplicity
  this->connect(this->mData, SIGNAL(multiplicityChanged(int, int)), SLOT(on_multiplicityChanged(int, int)));

  this->updateColorIcons(Settings::FontEditorOptions::foreColor(), Settings::FontEditorOptions::backColor());

  this->mData->setFont(this->ui->fontComboBox->currentFont());
  this->ui->lineEdit->setText(this->mData->characters());

  this->loadSettings();

  this->connectFontFamilyList(true);
}

DialogFontSelect::~DialogFontSelect()
{
  this->saveSettings();
  delete ui;
}

QString DialogFontSelect::characters()
{
  return this->mData->characters();
}

void DialogFontSelect::setCharacters(const QString &value)
{
  this->mData->setCharacters(value);
}

void DialogFontSelect::getFontParameters(Data::Containers::FontParameters *parameters)
{
  this->mData->getFontParameters(parameters);
}

void DialogFontSelect::setFontParameters(const Data::Containers::FontParameters &parameters)
{
  this->mData->setFontParameters(parameters);
}

void DialogFontSelect::loadSettings()
{
  Settings::AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("dialog-font-select");
  sett.beginGroup("toolbox");

  bool ok;
  int pageIndex = sett.value("pageIndex", QVariant("none")).toUInt(&ok);

  if (ok) {
    if (pageIndex < this->ui->toolBox->count()) {
      this->ui->toolBox->setCurrentIndex(pageIndex);
    }
  }

  sett.endGroup();
  sett.endGroup();
}

void DialogFontSelect::saveSettings() const
{
  Settings::AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("dialog-font-select");
  sett.beginGroup("toolbox");

  int pageIndex = this->ui->toolBox->currentIndex();
  sett.setValue("pageIndex", QVariant(pageIndex));

  sett.endGroup();
  sett.endGroup();
}

void DialogFontSelect::connectFontFamilyList(bool value)
{
  if (value) {
    this->connect(this->ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this->mData, SLOT(setFont(QFont)));
    this->mData->setFont(this->ui->fontComboBox->currentFont());
  } else {
    this->disconnect(this->ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this->mData, SLOT(setFont(QFont)));
  }
}

void DialogFontSelect::connectFontStyleList(bool value)
{
  if (value) {
    this->connect(this->ui->comboBoxStyle, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setStyle(QString)));
    this->mData->setStyle(this->ui->comboBoxStyle->currentText());
  } else {
    this->disconnect(this->ui->comboBoxStyle, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setStyle(QString)));
  }
}

void DialogFontSelect::connectFontSizeList(bool value)
{
  if (value) {
    this->connect(this->ui->comboBoxSize, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setSize(QString)));
    this->connect(this->ui->comboBoxSize, SIGNAL(currentTextChanged(QString)), this->mData, SLOT(setSize(QString)));
    this->mData->setSize(this->ui->comboBoxSize->currentText());
  } else  {
    this->disconnect(this->ui->comboBoxSize, SIGNAL(currentIndexChanged(QString)), this->mData, SLOT(setSize(QString)));
    this->disconnect(this->ui->comboBoxSize, SIGNAL(currentTextChanged(QString)), this->mData, SLOT(setSize(QString)));
  }
}

void DialogFontSelect::on_lineEdit_textChanged(const QString &value)
{
  QString stringNew = Parsing::Conversion::FontHelper::unescapeControlChars(value);
  QString stringOld = this->mData->characters();

  if (stringNew != stringOld) {
    this->mData->setCharacters(stringNew);
  }
}

void DialogFontSelect::on_tableView_doubleClicked(const QModelIndex &index)
{
  QString selected = this->ui->tableView->model()->data(index, Qt::DisplayRole).toString();
  this->mData->appendCharacters(selected);
}

void DialogFontSelect::on_pushButtonAppendSelected_clicked()
{
  QString selected = QString();

  QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();

  if (selectionModel->hasSelection()) {
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for (int i = 0; i < indexes.count(); i++) {
      QString a = this->ui->tableView->model()->data(indexes.at(i), Qt::DisplayRole).toString();
      selected += a;
    }
  }

  this->mData->appendCharacters(selected);
}

void DialogFontSelect::on_pushButtonAppendRange_clicked()
{
  DialogFontRange dialog(this);

  if (dialog.exec() == QDialog::Accepted) {
    QString selected = dialog.resultString();
    this->mData->appendCharacters(selected);
  }
}

void DialogFontSelect::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);

  QString str = this->mData->characters();
  QItemSelectionModel *selectionModel = this->ui->tableView->selectionModel();
  bool hasNew = false;

  if (selectionModel->hasSelection()) {
    QModelIndexList indexes = selectionModel->selectedIndexes();

    for (int i = 0; i < indexes.count(); i++) {
      QString a = this->ui->tableView->model()->data(indexes.at(i), Qt::DisplayRole).toString();

      if (!str.contains(a)) {
        hasNew = true;
        break;
      }
    }
  }

  this->ui->pushButtonAppendSelected->setEnabled(hasNew);
}

void DialogFontSelect::on_pushButtonForeColor_clicked()
{
  QColorDialog dialog(this->mData->foreground(), this);
  dialog.setOption(QColorDialog::ShowAlphaChannel, true);

  if (dialog.exec() == QDialog::Accepted) {
    Settings::FontEditorOptions::setForeColor(dialog.selectedColor());
    this->mData->setForeground(dialog.selectedColor());
  }
}

void DialogFontSelect::on_pushButtonBackColor_clicked()
{
  QColorDialog dialog(this->mData->background(), this);
  dialog.setOption(QColorDialog::ShowAlphaChannel, true);

  if (dialog.exec() == QDialog::Accepted) {
    Settings::FontEditorOptions::setBackColor(dialog.selectedColor());
    this->mData->setBackground(dialog.selectedColor());
  }
}

void DialogFontSelect::on_spinBoxMultiplicityHeight_valueChanged(int height)
{
  int width = this->ui->spinBoxMultiplicityWidth->value();
  this->mData->setMultiplicity(height, width);
}

void DialogFontSelect::on_spinBoxMultiplicityWidth_valueChanged(int width)
{
  int height = this->ui->spinBoxMultiplicityHeight->value();
  this->mData->setMultiplicity(height, width);
}

void DialogFontSelect::on_stylesListChanged(const QStringList &list, const QString &selected)
{
  this->connectFontStyleList(false);

  this->ui->comboBoxStyle->clear();
  this->ui->comboBoxStyle->addItems(list);

  int index = list.indexOf(selected);

  if (index >= 0) {
    this->ui->comboBoxStyle->setCurrentIndex(index);
  } else {
    this->ui->comboBoxStyle->setCurrentIndex(0);
  }

  this->connectFontStyleList(true);
}

void DialogFontSelect::on_sizesListChanged(const QList<int> &list, int selected)
{
  this->connectFontSizeList(false);

  int i = 0, previousSizeIndex = -1;
  QListIterator<int> it(list);
  it.toFront();
  this->ui->comboBoxSize->clear();

  while (it.hasNext()) {
    int size = it.next();

    this->ui->comboBoxSize->addItem(QString("%1").arg(size), QVariant(size));

    if (selected == 14 && previousSizeIndex == 0) {
      previousSizeIndex = i;
    }

    if (size == selected) {
      previousSizeIndex = i;
    }

    i++;
  }

  if (previousSizeIndex >= 0) {
    this->ui->comboBoxSize->setCurrentIndex(previousSizeIndex);
  } else {
    this->ui->comboBoxSize->setEditText(QString("%1").arg(selected));
  }

  this->connectFontSizeList(true);
}

void DialogFontSelect::on_charactersListChanged(const QString &value)
{
  QString stringNew = Parsing::Conversion::FontHelper::escapeControlChars(value);
  QString stringOld = this->ui->lineEdit->text();

  if (stringOld != stringNew) {
    this->ui->lineEdit->setText(stringNew);
  }
}

void DialogFontSelect::on_fontChanged(const QFont &value)
{
  this->ui->tableView->setFont(value);

  QFontMetrics metrics(value);

  int h = qMax(metrics.lineSpacing(), 15) + 2;
  int w = qMax(metrics.maxWidth(), 10);

  this->ui->tableView->verticalHeader()->hide();
  this->ui->tableView->horizontalHeader()->hide();
  this->ui->tableView->verticalHeader()->setDefaultSectionSize(h);
  this->ui->tableView->horizontalHeader()->setDefaultSectionSize(w);
  this->ui->tableView->verticalHeader()->show();
  this->ui->tableView->horizontalHeader()->show();

  QFont currentFont = this->ui->fontComboBox->currentFont();

  if ((currentFont.family() != value.family())) {
    this->ui->fontComboBox->setCurrentFont(value);
  }
}

void DialogFontSelect::on_monospacedChanged(bool value)
{
  this->ui->radioButtonMonospaced->setChecked(value);
  this->ui->radioButtonProportional->setChecked(!value);
}

void DialogFontSelect::on_fontMeasured(int count, const QSize &maxCharSize, const QSize &maxGlyphSize)
{
  this->ui->labelCharactersCount->setText(tr("Count: %1").arg(count));
  this->ui->labelCharactersMaxCharSize->setText(tr("Max size (w × h): %1 × %2").arg(maxCharSize.width()).arg(maxCharSize.height()));
  this->ui->labelCharactersMaxGlyphSize->setText(tr("Max size (w × h): %1 × %2").arg(maxGlyphSize.width()).arg(maxGlyphSize.height()));
}

void DialogFontSelect::updateColorIcons(const QColor &foreground, const QColor &background)
{
  QPixmap pixmapForeColor = QPixmap(24, 24);
  pixmapForeColor.fill(foreground);
  this->ui->pushButtonForeColor->setIcon(QIcon(pixmapForeColor));
  this->ui->pushButtonForeColor->setText(tr("Fore Color: %1").arg((quint32)foreground.rgba(), 8, 16, QChar('0')));

  QPixmap pixmapBackColor = QPixmap(24, 24);
  pixmapBackColor.fill(background);
  this->ui->pushButtonBackColor->setIcon(QIcon(pixmapBackColor));
  this->ui->pushButtonBackColor->setText(tr("Back Color: %1").arg((quint32)background.rgba(), 8, 16, QChar('0')));
}

void DialogFontSelect::on_multiplicityChanged(int height, int width)
{
  this->ui->spinBoxMultiplicityHeight->setValue(height);
  this->ui->spinBoxMultiplicityWidth->setValue(width);
}

} // namespace Fonts
} // namespace AppUI
