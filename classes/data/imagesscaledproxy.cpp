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

#include "imagesscaledproxy.h"

ImagesScaledProxy::ImagesScaledProxy(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    this->mScale = 2;
}

ImagesScaledProxy::~ImagesScaledProxy()
{
}

QVariant ImagesScaledProxy::data(const QModelIndex &index, int role) const
{
    Q_UNUSED(role)

    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}

int ImagesScaledProxy::scale() const
{
    return this->mScale;
}

void ImagesScaledProxy::setScale(int value)
{
    if (value >= 1)
    {
        this->mScale = value;
        emit this->scaleChanged();
    }
}
