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

#ifndef DIALOGOPTIONS_H
#define DIALOGOPTIONS_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogOptions;
}
//-----------------------------------------------------------------------------
class Preset;
class DataContainer;
class Parser;
class DialogPreview;
class SetupTabPrepare;
class SetupTabMatrix;
class SetupTabReordering;
class SetupTabImage;
class SetupTabFont;
class SetupTabTemplates;
//-----------------------------------------------------------------------------
class DialogOptions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogOptions(DataContainer *dataContainer, QWidget *parent = 0);
    virtual ~DialogOptions();

private:
    Ui::DialogOptions *ui;

    DataContainer *mData;

    Preset *mPreset;
    bool mPresetChanged;

    DialogPreview *mPreview;

    SetupTabPrepare    *mSetupPrepare;
    SetupTabMatrix     *mSetupMatrix;
    SetupTabReordering *mSetupReordering;
    SetupTabImage      *mSetupImage;
    SetupTabFont       *mSetupFont;
    SetupTabTemplates  *mSetupTemplates;

    void fillPresetsList();
    void presetLoad(const QString &name);
    void presetSaveAs(const QString &name);
    void presetRemove(const QString &name);
    void createPresetsDefault();
private slots:
    void presetChanged();
    void on_pushButtonPreview_clicked();
    void on_pushButtonPresetSaveAs_clicked();
    void on_pushButtonPresetRemove_clicked();
    void on_comboBoxPresets_currentIndexChanged(int index);
    void previewClosed();

protected:
    void done(int result);
};
//-----------------------------------------------------------------------------
#endif // DIALOGOPTIONS_H
