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

#ifndef DIALOGRESIZE_H
#define DIALOGRESIZE_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogResize;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class DialogResize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResize(int width, int height, int offsetX, int offsetY, bool center, bool allowChangeWidth, bool allowChangeHeight, QWidget *parent = 0);
    ~DialogResize();
    void getResizeInfo(int *width, int *height, int *offsetX, int *offsetY, bool *center, bool *allowChangeWidth, bool *allowChangeHeight);

private:
    Ui::DialogResize *ui;
    int mWidth;
    int mHeight;
    int mOffsetX;
    int mOffsetY;
    bool mCenter;
    bool mAllowChangeWidth;
    bool mAllowChangeHeight;
private slots:
    void on_checkBoxCenter_stateChanged(int state);
    void on_spinBoxWidth_valueChanged(int value);
    void on_spinBoxHeight_valueChanged(int value);
    void on_spinBoxOffsetX_valueChanged(int value);
    void on_spinBoxOffsetY_valueChanged(int value);
    void on_checkBoxResizeWidth_toggled(bool value);
    void on_checkBoxResizeHeight_toggled(bool value);
};
//-----------------------------------------------------------------------------
#endif // DIALOGRESIZE_H
