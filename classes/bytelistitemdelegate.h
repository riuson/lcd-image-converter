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

#ifndef BYTELISTITEMDELEGATE_H
#define BYTELISTITEMDELEGATE_H
//-----------------------------------------------------------------------------
#include <QtGui>
//-----------------------------------------------------------------------------
class ByteListItemDelegate : public QItemDelegate
{
public:
    ByteListItemDelegate(QObject *pobj);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    int bitsCount();
    void setBitsCount(const int value);
    QColor colorOdd() const;
    QColor colorEven() const;
    void setColorOdd(const QColor &value);
    void setColorEven(const QColor &value);
    void enableHighlightColors(bool value);
private:
    int mBitsCount;
    QColor mColorOdd;//1 3
    QColor mColorEven;//2 4
    bool mHighlightColors;
};
//-----------------------------------------------------------------------------
#endif // BYTELISTITEMDELEGATE_H
