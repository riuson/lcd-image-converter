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

#include "dialogoptions.h"
#include "ui_dialogoptions.h"

#include <QList>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include "datacontainer.h"
#include "dialogpreview.h"
#include "setuptabprepare.h"
#include "setuptabmatrix.h"
#include "setuptabreordering.h"
#include "setuptabimage.h"
#include "setuptabfont.h"
#include "setuptabtemplates.h"
#include "preset.h"
#include "filedialogoptions.h"

DialogOptions::DialogOptions(DataContainer *dataContainer, QWidget *parent) :
  QDialog(parent),
  ui(new Ui::DialogOptions)
{
  ui->setupUi(this);
  this->mPreview = NULL;

  this->mData = dataContainer;
  this->mPreset = new Preset(this);

  this->mSetupPrepare    = new SetupTabPrepare(this->mPreset, this);
  this->mSetupMatrix     = new SetupTabMatrix(this->mPreset, this);
  this->mSetupReordering = new SetupTabReordering(this->mPreset, this);
  this->mSetupImage      = new SetupTabImage(this->mPreset, this);
  this->mSetupFont       = new SetupTabFont(this->mPreset, this);
  this->mSetupTemplates  = new SetupTabTemplates(this->mPreset, this);

  QString selectedPreset = Preset::selectedName();
  int presetsCount = Preset::presetsList().length();

  // create default presets
  if (presetsCount == 0) {
    this->createPresetsDefault();
  } else {
    if (presetsCount == 1) {
      if (Preset::presetsList().at(0) == QString("default")) {
        this->createPresetsDefault();
      }
    }
  }

  this->mSetupPrepare->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->mSetupMatrix->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->mSetupReordering->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->mSetupImage->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->mSetupFont->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->mSetupTemplates->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
  this->connect(this->mPreset, SIGNAL(changed()), SLOT(presetChanged()));

  this->fillPresetsList();

  int presetIndex = this->ui->comboBoxPresets->findText(selectedPreset);

  if (presetIndex >= 0) {
    this->ui->comboBoxPresets->setCurrentIndex(presetIndex);
  }

  this->mPresetChanged = false;

  this->ui->tabWidgetSetupParts->addTab(this->mSetupPrepare, this->mSetupPrepare->windowTitle());
  this->ui->tabWidgetSetupParts->addTab(this->mSetupMatrix, this->mSetupMatrix->windowTitle());
  this->ui->tabWidgetSetupParts->addTab(this->mSetupReordering, this->mSetupReordering->windowTitle());
  this->ui->tabWidgetSetupParts->addTab(this->mSetupImage, this->mSetupImage->windowTitle());
  this->ui->tabWidgetSetupParts->addTab(this->mSetupFont, this->mSetupFont->windowTitle());
  this->ui->tabWidgetSetupParts->addTab(this->mSetupTemplates, this->mSetupTemplates->windowTitle());
}

DialogOptions::~DialogOptions()
{
  if (this->mPreview != NULL) {
    delete this->mPreview;
  }

  delete this->mSetupPrepare;
  delete this->mSetupMatrix;
  delete this->mSetupReordering;
  delete this->mSetupFont;
  delete this->mSetupImage;
  delete this->mSetupTemplates;
  delete ui;
  delete this->mPreset;
}

void DialogOptions::fillPresetsList(const QString &defaultName)
{
  this->disconnect(this->ui->comboBoxPresets, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxPresets_currentIndexChanged(int)));

  QString currentPresetName = this->ui->comboBoxPresets->currentText();
  int currentPresetIndex = this->ui->comboBoxPresets->currentIndex();

  this->ui->comboBoxPresets->clear();

  QStringList names = Preset::presetsList();
  this->ui->comboBoxPresets->addItems(names);

  bool defaultLoaded = false;

  if (!defaultName.isEmpty()) {
    if (names.contains(defaultName)) {
      this->ui->comboBoxPresets->setCurrentIndex(names.indexOf(defaultName));
      this->presetLoad(defaultName);
      defaultLoaded = true;
    }
  }

  if (!defaultLoaded) {
    if (names.contains(currentPresetName)) {
      this->ui->comboBoxPresets->setCurrentIndex(names.indexOf(currentPresetName));
      this->presetLoad(currentPresetName);
    } else if (currentPresetIndex < 0 && names.count() > 0) {
      this->ui->comboBoxPresets->setCurrentIndex(0);
      this->presetLoad(names.at(0));
    }
  }

  this->connect(this->ui->comboBoxPresets, SIGNAL(currentIndexChanged(int)), SLOT(on_comboBoxPresets_currentIndexChanged(int)));
}

void DialogOptions::presetLoad(const QString &name)
{
  if (this->mPreset->load(name)) {
    // update gui
  }
}

void DialogOptions::presetSaveAs(const QString &name)
{
  this->mPreset->save(name);
  this->fillPresetsList();

  int presetIndex = this->ui->comboBoxPresets->findText(name);

  if (presetIndex >= 0) {
    this->ui->comboBoxPresets->setCurrentIndex(presetIndex);
  }
}

void DialogOptions::presetRemove(const QString &name)
{
  Preset::remove(name);

  this->fillPresetsList();
}

void DialogOptions::createPresetsDefault()
{
  Preset matrix(this);

  matrix.initMono(MonochromeTypeDiffuseDither);
  matrix.save(tr("Monochrome"));

  matrix.initGrayscale(4);
  matrix.save(tr("Grayscale 4"));

  matrix.initGrayscale(8);
  matrix.save(tr("Grayscale 8"));

  matrix.initColor(0, 4, 5, 4);
  matrix.save(tr("Color R4G5B4"));

  matrix.initColor(0, 5, 6, 5);
  matrix.save(tr("Color R5G6B5"));

  matrix.initColor(0, 8, 8, 8);
  matrix.save(tr("Color R8G8B8"));

  matrix.initColor(8, 8, 8, 8);
  matrix.save(tr("Color A8R8G8B8"));
}

bool DialogOptions::checkOverwrite(const QString &originalName, QString *resultName) const
{
  QStringList existingNames = Preset::presetsList();

  if (!existingNames.contains(originalName)) {
    *resultName = originalName;
    return true;
  }

  QInputDialog dialog(this->parentWidget());
  QObject::connect(&dialog, SIGNAL(textValueChanged(QString)), this, SLOT(presetOverwiteNameChanged(QString)));
  dialog.setWindowTitle(tr("Import - Attention"));
  dialog.setTextValue(originalName);

  if (dialog.exec() == QDialog::Accepted) {
    *resultName = dialog.textValue();
    return true;
  }

  return false;
}

void DialogOptions::presetChanged()
{
  if (this->mData != NULL) {
    if (this->mPreview != NULL) {
      this->mPreview->updatePreview();
    }
  }

  this->mPresetChanged = true;
}

void DialogOptions::on_pushButtonPreview_clicked()
{
  if (this->mPreview == NULL) {
    this->mPreview = new DialogPreview(this->mData, this->mPreset, this);
    QObject::connect(this->mPreview, SIGNAL(accepted()), this, SLOT(previewClosed()));
    QObject::connect(this->mPreview, SIGNAL(rejected()), this, SLOT(previewClosed()));
  }

  this->mPreview->show();
}

void DialogOptions::on_pushButtonPresetSaveAs_clicked()
{
  QStringList names = Preset::presetsList();

  QInputDialog dialog(this);
  dialog.setComboBoxItems(names);
  dialog.setComboBoxEditable(true);

  QString current = this->ui->comboBoxPresets->currentText();
  bool ok;

  QString result = dialog.getItem(this, tr("Enter preset name"), tr("Preset name:"), names, names.indexOf(current), true, &ok);

  if (ok && !result.isEmpty()) {
    this->presetSaveAs(result);
  }

  this->fillPresetsList();
}

void DialogOptions::on_pushButtonPresetRemove_clicked()
{
  QString name = this->ui->comboBoxPresets->currentText();
  this->presetRemove(name);

  this->fillPresetsList();
}

void DialogOptions::on_pushButtonPresetImport_clicked()
{
  QFileDialog dialog(this->parentWidget());
  dialog.setAcceptMode(QFileDialog::AcceptOpen);
  dialog.setDirectory(FileDialogOptions::directory(FileDialogOptions::Dialogs::ImportPreset));
  dialog.setFileMode(QFileDialog::ExistingFiles);
  dialog.setNameFilter(tr("XML Files (*.xml)"));
  dialog.setWindowTitle(tr("Open xml preset file"));

  if (dialog.exec() == QDialog::Accepted) {
    FileDialogOptions::setDirectory(FileDialogOptions::Dialogs::ImportPreset, dialog.directory().absolutePath());
    QString filename = dialog.selectedFiles().at(0);

    Preset *importedPreset = new Preset(this);
    importedPreset->loadXML(filename);
    QString resultPresetName;

    if (this->checkOverwrite(importedPreset->name(), &resultPresetName)) {
      importedPreset->save(resultPresetName);
    }

    delete importedPreset;

    this->fillPresetsList(resultPresetName);
  }
}

void DialogOptions::on_pushButtonPresetExport_clicked()
{
  QFileDialog dialog(this->parentWidget());
  dialog.setAcceptMode(QFileDialog::AcceptSave);
  dialog.setDirectory(FileDialogOptions::directory(FileDialogOptions::Dialogs::ExportPreset));
  dialog.setFileMode(QFileDialog::AnyFile);
  dialog.setNameFilter(tr("XML Files (*.xml)"));
  dialog.setDefaultSuffix(QString("xml"));
  dialog.setWindowTitle(tr("Save file as"));
  dialog.selectFile(this->mPreset->name());

  if (dialog.exec() == QDialog::Accepted) {
    FileDialogOptions::setDirectory(FileDialogOptions::Dialogs::ExportPreset, dialog.directory().absolutePath());
    QString filename = dialog.selectedFiles().at(0);
    this->mPreset->saveXML(filename);
  }
}

void DialogOptions::on_comboBoxPresets_currentIndexChanged(int index)
{
  QString name = this->ui->comboBoxPresets->itemText(index);
  this->presetLoad(name);
}

void DialogOptions::previewClosed()
{
  if (this->mPreview != NULL) {
    delete this->mPreview;
    this->mPreview = NULL;
  }
}

void DialogOptions::presetOverwiteNameChanged(const QString &value)
{
  QInputDialog *dialog = qobject_cast<QInputDialog *>(sender());
  QStringList existingNames = Preset::presetsList();

  if (existingNames.contains(value)) {
    QString message = tr("Preset with name \"%1\" already exists. Continue with overwrite?", "Warning about preset overwrite").arg(value);
    dialog->setLabelText(message);
  } else {
    QString message = tr("Preset with name \"%1\" doesn't exists. All OK.", "Warning about preset overwrite").arg(value);
    dialog->setLabelText(message);
  }
}

void DialogOptions::done(int result)
{
  if (result == QDialog::Accepted) {
    if (this->mPresetChanged) {
      QMessageBox msgBox;
      msgBox.setText(tr("Save changes?"));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      msgBox.setDefaultButton(QMessageBox::Cancel);
      int result = msgBox.exec();

      switch (result) {
        case QMessageBox::Yes: {
          QString name = this->ui->comboBoxPresets->currentText();

          if (name.isEmpty()) {
            name = QString("default");
          }

          this->mPreset->save(name);
          Preset::setSelectedName(name);

          QDialog::done(result);
          break;
        }

        case QMessageBox::No: {
          QDialog::done(result);
          break;
        }

        case QMessageBox::Cancel: {
          break;
        }

        default: {
          QDialog::done(result);
          break;
        }
      }
    } else {
      QDialog::done(result);
    }
  } else {
    QDialog::done(result);
    return;
  }
}

