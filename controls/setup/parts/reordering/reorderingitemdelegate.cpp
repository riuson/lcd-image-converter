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

#include "reorderingitemdelegate.h"

ReorderingItemDelegate::ReorderingItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    this->mColor= QColor("gold");
}

void ReorderingItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    if (index.isValid())
    {
        int rows = index.model()->rowCount();
        QRect rect = option.rect;
        int row = index.row();
        if (row == 0)
        {
            //painter->setPen(this->mColorOdd);
            QPen pen(QBrush(this->mColor), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.bottom() - 1, rect.right(), rect.bottom() - 1);
        }
        if (row == rows - 1)
        {
            QPen pen(QBrush(this->mColor), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.top() + 1, rect.right(), rect.top() + 1);
        }
    }
    QItemDelegate::paint(painter, option, index);
}

QColor ReorderingItemDelegate::color() const
{
    return this->mColor;
}

void ReorderingItemDelegate::setColor(const QColor &value)
{
    this->mColor = value;
}

