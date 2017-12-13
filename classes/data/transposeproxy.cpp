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

#include "transposeproxy.h"

namespace Data
{
namespace Models
{

TransposeProxy::TransposeProxy(QObject *parent)
  : QSortFilterProxyModel(parent)
{
}

QVariant TransposeProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (this->sourceModel() == nullptr) {
    return QVariant();
  }

  return this->sourceModel()->headerData(section, orientation == Qt::Vertical ? Qt::Horizontal : Qt::Vertical, role);
}

QModelIndex TransposeProxy::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return this->createIndex(row, column);
}

QModelIndex TransposeProxy::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
  return QModelIndex();
}

int TransposeProxy::rowCount(const QModelIndex &parent) const
{
  if (this->sourceModel() == nullptr) {
    return 0;
  }

  return this->sourceModel()->columnCount(parent);
}

int TransposeProxy::columnCount(const QModelIndex &parent) const
{
  if (this->sourceModel() == nullptr) {
    return 0;
  }

  return this->sourceModel()->rowCount(parent);
}

QModelIndex TransposeProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
  return this->index(sourceIndex.column(), sourceIndex.row(), sourceIndex.parent());
}

QModelIndex TransposeProxy::mapToSource(const QModelIndex &proxyIndex) const
{
  if (sourceModel() != nullptr && proxyIndex.isValid()) {
    return sourceModel()->index(proxyIndex.column(), proxyIndex.row(), proxyIndex.parent());
  } else {
    return QModelIndex();
  }
}

} // namespace Models
} // namespace Data
