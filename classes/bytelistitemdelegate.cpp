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

#include "bytelistitemdelegate.h"
//-----------------------------------------------------------------------------
ByteListItemDelegate::ByteListItemDelegate(QObject *pobj = 0) : QItemDelegate(pobj)
{
    this->mBitsCount = 8;
    this->mColorEven = QColor("silver");
    this->mColorOdd = QColor("black");
    this->mHighlightColors = false;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    if (index.isValid())
    {
        QRect rect = option.rect;
        bool even = (index.row() / this->mBitsCount) % 2;
        if (even)
        {
            //painter->setPen(this->mColorEven);
            QPen pen(QBrush(this->mColorEven), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.top() + 2, rect.right(), rect.top() + 2);
        }
        else
        {
            //painter->setPen(this->mColorOdd);
            QPen pen(QBrush(this->mColorOdd), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.bottom() - 2, rect.right(), rect.bottom() - 2);
        }
        if (this->mHighlightColors)
        {
            QString d = index.data().toString();
            if (d.contains("R", Qt::CaseInsensitive))
            {
                QPen pen(QBrush(QColor("red")), 3);
                painter->setPen(pen);
            }
            if (d.contains("G", Qt::CaseInsensitive))
            {
                QPen pen(QBrush(QColor("green")), 3);
                painter->setPen(pen);
            }
            if (d.contains("B", Qt::CaseInsensitive))
            {
                QPen pen(QBrush(QColor("blue")), 3);
                painter->setPen(pen);
            }
            painter->drawLine(rect.left(), rect.top() + 10, rect.right(), rect.top() + 10);
            painter->drawLine(rect.left(), rect.bottom() - 10, rect.right(), rect.bottom() - 10);
        }
    }
    QItemDelegate::paint(painter, option, index);
}
//-----------------------------------------------------------------------------
int ByteListItemDelegate::bitsCount()
{
    return this->mBitsCount;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setBitsCount(const int value)
{
    this->mBitsCount = value;
}
//-----------------------------------------------------------------------------
QColor ByteListItemDelegate::colorOdd() const
{
    return this->mColorOdd;
}
//-----------------------------------------------------------------------------
QColor ByteListItemDelegate::colorEven() const
{
    return this->mColorEven;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setColorOdd(const QColor &value)
{
    this->mColorOdd = value;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setColorEven(const QColor &value)
{
    this->mColorEven = value;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::enableHighlightColors(bool value)
{
    this->mHighlightColors = value;
}
//-----------------------------------------------------------------------------
