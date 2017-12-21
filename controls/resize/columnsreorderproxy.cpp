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

#include "columnsreorderproxy.h"
#include <QDebug>

namespace Data
{
namespace Models
{

ColumnsReorderProxy::ColumnsReorderProxy(QObject *parent)
  : QSortFilterProxyModel(parent)
{
  this->mFromColumn = 0;
  this->mToColumn = 0;
}
/*
QVariant ColumnsReorderProxy::headerData(int section, Qt::Orientation orientation, int role) const
{

}

QVariant ColumnsReorderProxy::data(const QModelIndex &index, int role) const
{

}
*/
QModelIndex ColumnsReorderProxy::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return this->createIndex(row, column);
}

QModelIndex ColumnsReorderProxy::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
  return QModelIndex();
}

QModelIndex ColumnsReorderProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
  return this->index(sourceIndex.row(), this->columnFromSource(sourceIndex.column()), sourceIndex.parent());
}

QModelIndex ColumnsReorderProxy::mapToSource(const QModelIndex &proxyIndex) const
{
  if ((this->sourceModel() != nullptr) && proxyIndex.isValid()) {
    return this->sourceModel()->index(proxyIndex.row(), this->columnToSource(proxyIndex.column()), proxyIndex.parent());
  } else {
    return QModelIndex();
  }
}

void ColumnsReorderProxy::setReorder(int oldPosition, int newPosition)
{
  this->mListFromSource.clear();
  this->mListToSource.clear();

  int count = this->sourceModel() != nullptr ? this->sourceModel()->columnCount() : 0;

  if ((newPosition < count) && (oldPosition < count)) {
    for (int i = 0; i < count; i++) {
      this->mListFromSource.append(i);
      this->mListToSource.append(i);
    }

    this->mListFromSource.move(newPosition, oldPosition);
    this->mListToSource.move(oldPosition, newPosition);
  }
}

int ColumnsReorderProxy::columnFromSource(int value) const
{
  if (value < 0) {
    return value;
  }

  return this->mListFromSource[value];
}

int ColumnsReorderProxy::columnToSource(int value) const
{
  if (value < 0) {
    return value;
  }

  return this->mListToSource[value];
}

} // namespace Models
} // namespace Data
