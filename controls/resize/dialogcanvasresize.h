/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef DIALOGCANVASRESIZE_H
#define DIALOGCANVASRESIZE_H

#include <QDialog>

namespace Ui {
class DialogCanvasResize;
}

class QItemSelection;
class DataContainer;
class ImagesModel;
class ImagesScaledProxy;
class ImagesResizedProxy;
class ImagesFilterProxy;
class ColumnsReorderProxy;
class TransposeProxy;

class DialogCanvasResize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCanvasResize(DataContainer *container, QWidget *parent = 0);
    ~DialogCanvasResize();

    void selectKeys(const QStringList &keys);
    void resizeInfo(int *left, int *top, int *right, int *bottom) const;
    void setResizeInfo(int left, int top, int right, int bottom);

private:
    Ui::DialogCanvasResize *ui;

    DataContainer *mContainer;
    ImagesModel *mModel;
    ImagesScaledProxy *mScaledProxy;
    ImagesResizedProxy *mResizedProxy;
    ImagesFilterProxy *mFilter;
    ColumnsReorderProxy *mReorderProxy;
    TransposeProxy *mTranspose;

    int mLeft;
    int mTop;
    int mRight;
    int mBottom;

private slots:
    void spinBox_valueChanged(int value);
    void on_spinBoxScale_valueChanged(int value);
    void on_pushButtonReset_clicked();
    void resizeToContents();
};

#endif // DIALOGCANVASRESIZE_H
