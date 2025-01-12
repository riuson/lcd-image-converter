/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2022 riuson
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

#include "alignmodproxy.h"
#include "alignmodinfo.h"
#include "bitmaphelper.h"
#include "imagesmodel.h"
#include <QColor>

namespace Data
{
namespace Models
{

AlignModProxy::AlignModProxy(AlignModInfo *alignModInfo, QObject *parent)
  : QSortFilterProxyModel(parent),
    mAlignModInfo(alignModInfo)
{
}

QVariant AlignModProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant result = this->sourceModel()->headerData(section, orientation, role);

  if (role == Qt::DisplayRole) {
    int sourceColumns = this->sourceModel()->columnCount();

    if (orientation == Qt::Horizontal) {
      if (section == sourceColumns + 0) {
        result = tr("Size");
      }
    }
  }

  return result;
}

int AlignModProxy::columnCount(const QModelIndex &parent) const
{
  if (this->sourceModel() == nullptr) {
    return 0;
  }

  int sourceColumns = this->sourceModel()->columnCount(parent);
  return sourceColumns + 1;
}

QVariant AlignModProxy::data(const QModelIndex &index, int role) const
{
  QVariant result = this->sourceModel()->data(index, role);

  if (!index.isValid()) {
    return result;
  }

  int sourceColumns = this->sourceModel()->columnCount();
  int columnIndex = index.column();

  switch (role) {
    case Qt::DecorationRole: {
      if (columnIndex == 1) {
        QImage imageSource = result.value<QImage>();
        QImage imageAligned = Parsing::Conversion::BitmapHelper::align(
                                &imageSource,
                                this->mAlignModInfo->summary().horizontalMode,
                                this->mAlignModInfo->summary().horizontalOffset,
                                this->mAlignModInfo->summary().verticalMode,
                                this->mAlignModInfo->summary().verticalOffset);
        result = imageAligned;
      }

      break;
    }

    case Qt::SizeHintRole: {
      if (columnIndex == 1) {
        QSize size = result.toSize();
        result = size;
      }

      break;
    }

    case Qt::DisplayRole: {
      if (columnIndex == sourceColumns + 0) {
        QVariant varSize = this->sourceModel()->data(index, Data::Models::ImagesModel::ImageSizeRole);
        QSize size = varSize.toSize();
        result = QString("%1x%2").arg(size.width()).arg(size.height());
      }

      break;
    }

    default:
      break;
  }

  return result;
}

QModelIndex AlignModProxy::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return this->createIndex(row, column);
}

QModelIndex AlignModProxy::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
  return QModelIndex();
}

QModelIndex AlignModProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
  return this->index(sourceIndex.row(), sourceIndex.column(), sourceIndex.parent());
}

QModelIndex AlignModProxy::mapToSource(const QModelIndex &proxyIndex) const
{
  if (sourceModel() && proxyIndex.isValid()) {
    return sourceModel()->index(proxyIndex.row(), proxyIndex.column(), proxyIndex.parent());
  } else {
    return QModelIndex();
  }
}

// void AlignModProxy::notifyPreviewChanged()
// {
//   emit this->dataChanged(
//     index(0, 1),
//     index(this->rowCount() - 1, 1));
// }

} // namespace Models
} // namespace Data
