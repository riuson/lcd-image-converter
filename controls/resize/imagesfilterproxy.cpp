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

#include "imagesfilterproxy.h"

#include <QStringList>
//-----------------------------------------------------------------------------
ImagesFilterProxy::ImagesFilterProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    this->mKeys = new QStringList();
}
//-----------------------------------------------------------------------------
ImagesFilterProxy::~ImagesFilterProxy()
{
    delete this->mKeys;
}
//-----------------------------------------------------------------------------
bool ImagesFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = this->sourceModel()->index(source_row, 0, source_parent);
    QString name = this->sourceModel()->data(index).toString();

    return (this->mKeys->contains(name, Qt::CaseSensitive));
}
//-----------------------------------------------------------------------------
void ImagesFilterProxy::setFilter(const QStringList &keys)
{
    emit this->beginResetModel();

    this->mKeys->clear();
    this->mKeys->append(keys);

    emit this->endResetModel();
}
//-----------------------------------------------------------------------------
