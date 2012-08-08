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

#ifndef DIALOGPREVIEW_H
#define DIALOGPREVIEW_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
template <class T> class QList;
//-----------------------------------------------------------------------------
namespace Ui {
class DialogPreview;
}
class Converter;
class IDataContainer;
//-----------------------------------------------------------------------------
class DialogPreview : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPreview(IDataContainer *dataContainer, QList<quint32> *matrix, QWidget *parent = 0);
    ~DialogPreview();
    void updatePreview();
    
private:
    Ui::DialogPreview *ui;

    IDataContainer *mData;
    QImage mImageOriginal;
    QImage mImageProcessed;
    QList<quint32> *mMatrix;

private slots:
    void on_comboBoxSampleKey_currentIndexChanged();
};
//-----------------------------------------------------------------------------
#endif // DIALOGPREVIEW_H
