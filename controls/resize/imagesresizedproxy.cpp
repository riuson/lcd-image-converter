/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "imagesresizedproxy.h"
#include "bitmaphelper.h"
#include <QColor>

ImagesResizedProxy::ImagesResizedProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    this->setCrop(0, 0, 0, 0);
}

QVariant ImagesResizedProxy::data(const QModelIndex &index, int role) const
{
    QVariant result = this->sourceModel()->data(index, role);

    if (!index.isValid())
        return result;


    int columnIndex = index.column();

    switch (role)
    {
    case Qt::DecorationRole:
    {
        if (columnIndex == 1)
        {
            QImage imageSource = result.value<QImage>();
            QColor backgroundColor = BitmapHelper::detectBackgroundColor(&imageSource);
            QImage imageScaled = BitmapHelper::crop(&imageSource, this->mLeft, this->mTop, this->mRight, this->mBottom, backgroundColor);
            result = imageScaled;
        }
        break;
    }
    case Qt::SizeHintRole:
    {
        if (columnIndex == 1)
        {
            QSize size = result.toSize();
            size.rwidth() += this->mLeft + this->mRight;
            size.rheight() += this->mTop + this->mBottom;

            if (size.height() < 1)
            {
                size.setHeight(1);
            }

            if (size.width() < 1)
            {
                size.setWidth(1);
            }

            result = size;
        }
        break;
    }
    default:
        break;
    }

    return result;
}

void ImagesResizedProxy::setCrop(int left, int top, int right, int bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;

    QModelIndex indexFrom = this->index(0, 0, QModelIndex());
    QModelIndex indexTo = this->index(this->rowCount() - 1, this->columnCount() - 1, QModelIndex());
    emit this->dataChanged(indexFrom, indexTo);
}
