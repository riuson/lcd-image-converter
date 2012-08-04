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

#ifndef DIALOGCONVERT_H
#define DIALOGCONVERT_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogConvert;
}
class Converter;
class IDataContainer;
//-----------------------------------------------------------------------------
class DialogConvert : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConvert(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogConvert();

private:
    Ui::DialogConvert *ui;
    QWidget *mWidgetSetup;
    Converter *mConverter;
    IDataContainer *mData;
    QImage mImageOriginal;
    QImage mImageProcessed;
private slots:
    void on_comboBoxType_currentIndexChanged();
    void on_comboBoxSampleKey_currentIndexChanged();
    void on_comboBoxDataLength_currentIndexChanged();
    void on_comboBoxRotate_currentIndexChanged(int index);
    void on_checkBoxFlipHorizontal_toggled(bool value);
    void on_checkBoxFlipVertical_toggled(bool value);
    void on_checkBoxInverse_toggled(bool value);
    void on_checkBoxPack_toggled(bool value);
    void on_radioButtonAlignHigh_toggled(bool value);
    void updatePreview();
signals:
    void dataLengthChanged(int length);
    void dataPackChanged(bool pack);
    void dataAlignChanged(int align);
    void swapBytesChanged(bool swap);
    void mirrorBytesChanged(bool mirror);
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT_H
