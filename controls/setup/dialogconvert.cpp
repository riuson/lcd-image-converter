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

#include "dialogconvert.h"
#include "ui_dialogconvert.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QSettings>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include "idatacontainer.h"
#include "dialogpreview.h"
#include "setuptabprepare.h"
#include "setuptabmatrix.h"
#include "setuptabimage.h"
#include "preset.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);
    this->mPreview = NULL;

    this->mData = dataContainer;
    this->mPreset = new Preset(this);

    this->mSetupPrepare = new SetupTabPrepare(this->mPreset, this);
    this->mSetupMatrix = new SetupTabMatrix(this->mPreset, this);
    this->mSetupImage = new SetupTabImage(this->mPreset, this);

    QSettings sett;
    sett.beginGroup("presets");
    QString selectedPreset = sett.value("selected", QVariant("")).toString();
    int presetsCount = sett.childGroups().count();
    sett.endGroup();

    if (presetsCount == 0)
        this->createPresetsDefault();

    this->mSetupPrepare->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
    this->mSetupMatrix->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
    this->mSetupImage->connect(this->mPreset, SIGNAL(changed()), SLOT(matrixChanged()));
    this->connect(this->mPreset, SIGNAL(changed()), SLOT(updatePreview()));

    this->fillPresetsList();

    int presetIndex = this->ui->comboBoxPresets->findText(selectedPreset);
    if (presetIndex >= 0)
        this->ui->comboBoxPresets->setCurrentIndex(presetIndex);

    this->mPresetChanged = false;

    this->ui->tabWidgetSetupParts->addTab(this->mSetupPrepare, SetupTabPrepare::title());
    this->ui->tabWidgetSetupParts->addTab(this->mSetupMatrix, SetupTabMatrix::title());
    this->ui->tabWidgetSetupParts->addTab(this->mSetupImage, SetupTabImage::title());
}
//-----------------------------------------------------------------------------
DialogConvert::~DialogConvert()
{

    if (this->mPreview != NULL)
        delete this->mPreview;

    delete ui;
    delete this->mPreset;
}
//-----------------------------------------------------------------------------
void DialogConvert::fillPresetsList()
{
    QString current = this->ui->comboBoxPresets->currentText();

    this->ui->comboBoxPresets->clear();

    QSettings sett;
    sett.beginGroup("presets");
    QStringList names = sett.childGroups();
    sett.endGroup();

    this->ui->comboBoxPresets->addItems(names);

    if (names.contains(current))
    {
        this->ui->comboBoxPresets->setCurrentIndex(names.indexOf(current));
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::presetLoad(const QString &name)
{
    if (this->mPreset->load(name))
    {
        // update gui
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::presetSaveAs(const QString &name)
{
    this->mPreset->save(name);
    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::presetRemove(const QString &name)
{
    QSettings sett;
    sett.beginGroup("presets");

    sett.beginGroup(name);
    sett.remove("");

    sett.endGroup();

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::createPresetsDefault()
{
    Preset matrix(this);

    matrix.initMono(MonochromeTypeDiffuseDither);
    matrix.save(tr("Monochrome"));

    matrix.initGrayscale(8);
    matrix.save(tr("Grayscale 8"));

    matrix.initColor(4, 5, 4);
    matrix.save(tr("Color R4G5B4"));

    matrix.initColor(5, 6, 5);
    matrix.save(tr("Color R5G6B5"));

    matrix.initColor(8, 8, 8);
    matrix.save(tr("Color R8G8B8"));
}
//-----------------------------------------------------------------------------
void DialogConvert::updatePreview()
{
    if (this->mData != NULL)
    {
        if (this->mPreview != NULL)
            this->mPreview->updatePreview();

        this->mPresetChanged = true;
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPreview_clicked()
{
    if (this->mPreview == NULL)
    {
        this->mPreview = new DialogPreview(this->mData, this->mPreset, this);
        QObject::connect(this->mPreview, SIGNAL(accepted()), this, SLOT(previewClosed()));
        QObject::connect(this->mPreview, SIGNAL(rejected()), this, SLOT(previewClosed()));
    }

    this->mPreview->show();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPresetSaveAs_clicked()
{
    QSettings sett;
    sett.beginGroup("presets");

    QStringList names = sett.childGroups();

    sett.endGroup();

    QInputDialog dialog(this);
    dialog.setComboBoxItems(names);
    dialog.setComboBoxEditable(true);

    QString current = this->ui->comboBoxPresets->currentText();
    bool ok;

    QString result = dialog.getItem(this, tr("Enter preset name"), tr("Preset name:"), names, names.indexOf(current), true, &ok);
    if (ok && !result.isEmpty())
    {
        this->presetSaveAs(result);
    }

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_pushButtonPresetRemove_clicked()
{
    QString name = this->ui->comboBoxPresets->currentText();
    this->presetRemove(name);

    this->fillPresetsList();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxPresets_currentIndexChanged(int index)
{
    QString name = this->ui->comboBoxPresets->itemText(index);
    this->presetLoad(name);
}
//-----------------------------------------------------------------------------
void DialogConvert::previewClosed()
{
    if (this->mPreview != NULL)
    {
        delete this->mPreview;
        this->mPreview = NULL;
    }
}
//-----------------------------------------------------------------------------
void DialogConvert::matrixChanged()
{
    this->mPresetChanged = true;
}
//-----------------------------------------------------------------------------
void DialogConvert::done(int result)
{
    if (result == QDialog::Accepted)
    {
        if (this->mPresetChanged)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Save changes?"));
            msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int result = msgBox.exec();

            switch (result)
            {
            case QMessageBox::Yes:
            {
                QString name = this->ui->comboBoxPresets->currentText();

                this->mPreset->save(name);

                QSettings sett;
                sett.beginGroup("presets");
                sett.setValue("selected", name);
                sett.endGroup();

                QDialog::done(result);
                break;
            }
            case QMessageBox::No:
            {
                QDialog::done(result);
                break;
            }
            case QMessageBox::Cancel:
            {
                break;
            }
            default:
            {
                QDialog::done(result);
                break;
            }
            }
        }
        else
        {
            QDialog::done(result);
        }
    }
    else
    {
        QDialog::done(result);
        return;
    }
}
//-----------------------------------------------------------------------------
