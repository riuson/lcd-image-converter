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

#ifndef DIALOGCONVERT2_H
#define DIALOGCONVERT2_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogConvert;
}
//-----------------------------------------------------------------------------
class ConversionMatrix;
class IDataContainer;
class Parser;
class DialogPreview;
class SetupTabPrepare;
class SetupTabMatrix;
class SetupTabImage;
//-----------------------------------------------------------------------------
class DialogConvert : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConvert(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogConvert();

private:
    Ui::DialogConvert *ui;

    IDataContainer *mData;

    ConversionMatrix *mMatrix;
    bool mMatrixChanged;

    DialogPreview *mPreview;

    SetupTabPrepare *mSetupPrepare;
    SetupTabMatrix  *mSetupMatrix;
    SetupTabImage   *mSetupImage;

    void fillPresetsList();
    void presetLoad(const QString &name);
    void presetSaveAs(const QString &name);
    void presetRemove(const QString &name);
    void createPresetsDefault();
private slots:
    void updatePreview();
    void on_pushButtonPreview_clicked();
    void on_pushButtonPresetSaveAs_clicked();
    void on_pushButtonPresetRemove_clicked();
    void on_comboBoxPresets_currentIndexChanged(int index);
    void previewClosed();

    void matrixChanged();

protected:
    void done(int result);
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT2_H
