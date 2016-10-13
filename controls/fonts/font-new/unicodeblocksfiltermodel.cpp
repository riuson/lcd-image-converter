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

#include "unicodeblocksfiltermodel.h"

UnicodeBlocksFilterModel::UnicodeBlocksFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    this->mName = QString();
}

bool UnicodeBlocksFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (this->mName.isEmpty())
        return true;
    QModelIndex index = this->sourceModel()->index(source_row, 0, source_parent);
    QString name = this->sourceModel()->data(index).toString();

    return (name.contains(this->mName, Qt::CaseInsensitive));
}

void UnicodeBlocksFilterModel::setNameFilter(const QString &name)
{
    this->beginResetModel();

    this->mName = name;

    this->endResetModel();
}

