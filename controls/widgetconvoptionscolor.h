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

#ifndef WIDGETCONVOPTIONSCOLOR_H
#define WIDGETCONVOPTIONSCOLOR_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsColor;
}
class ByteListItemDelegate;
class QButtonGroup;
class ConverterColor;
class IConverter;
//-----------------------------------------------------------------------------
class WidgetConvOptionsColor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsColor(IConverter *options, QWidget *parent = 0);
    ~WidgetConvOptionsColor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsColor *ui;
    ByteListItemDelegate *mDelegate;
    ConverterColor *mConv;
    bool mReady;
private slots:
    void updatePreview();
public slots:
    void dataLengthChanged(int value);
    void dataPackChanged(bool value);
    void swapBytesChanged(bool value);
    void mirrorBytesChanged(bool value);
signals:
    void settingsChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSCOLOR_H
