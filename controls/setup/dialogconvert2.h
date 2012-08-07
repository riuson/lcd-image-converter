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
template <class T> class QList;
//-----------------------------------------------------------------------------
namespace Ui {
class DialogConvert2;
}
class IDataContainer;
class Converter;
class MatrixPreviewModel;
class QTableView;
//-----------------------------------------------------------------------------
class DialogConvert2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConvert2(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogConvert2();
    
private:
    Ui::DialogConvert2 *ui;

    IDataContainer *mData;

    QList<quint32> *mMatrix;
    MatrixPreviewModel *mMatrixModel;

    void setTableHeight(QTableView *tableView);

private slots:
    void updatePreview();
    void on_pushButtonPreview_clicked();
    void on_comboBoxConversionType_currentIndexChanged(int index);
    void on_comboBoxMonochromeType_currentIndexChanged(int index);
    void on_comboBoxBlockSize_currentIndexChanged(int index);
    void on_comboBoxRotate_currentIndexChanged(int index);
    void on_checkBoxFlipHorizontal_toggled(bool value);
    void on_checkBoxFlipVertical_toggled(bool value);
    void on_checkBoxInverse_toggled(bool value);
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT2_H
