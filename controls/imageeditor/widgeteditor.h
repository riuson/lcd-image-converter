/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#ifndef WIDGETEDITOR_H
#define WIDGETEDITOR_H
//-----------------------------------------------------------------------------
#include <QWidget>
#include "iimageeditor.h"
//-----------------------------------------------------------------------------
namespace Ui {
class WidgetEditor;
}
//-----------------------------------------------------------------------------
namespace ImageEditor
{
class WidgetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetEditor(QWidget *parent = 0);
    ~WidgetEditor();

    const QImage *image() const;
    void setImage(const QImage *_value);

    const QColor foreColor() const;
    void setForeColor(QColor value) const;

    const QColor backColor() const;
    void setBackColor(QColor value) const;

signals:
    void imageChanged(const QImage *_image);
    void scaleChanged(int _scale);
    void mouseMoved(const QPoint *_point);

private:
    Ui::WidgetEditor *ui;
};
}
//-----------------------------------------------------------------------------
#endif // WIDGETIMAGEEDITOR_H
