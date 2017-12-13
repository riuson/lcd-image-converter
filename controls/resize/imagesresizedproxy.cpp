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
#include "imagesmodel.h"
#include <QColor>

namespace Data
{
namespace Models
{

ImagesResizedProxy::ImagesResizedProxy(QObject *parent)
  : QSortFilterProxyModel(parent)
{
  this->mLeft = 0;
  this->mRight = 0;
  this->mTop = 0;
  this->mBottom = 0;
}

QVariant ImagesResizedProxy::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant result = this->sourceModel()->headerData(section, orientation, role);

  if (role == Qt::DisplayRole) {
    int sourceColumns = this->sourceModel()->columnCount();

    if (orientation == Qt::Horizontal) {
      if (section == sourceColumns + 0) {
        result = tr("Original size");
      } else if (section == sourceColumns + 1) {
        result = tr("New size");
      }
    }
  }

  return result;
}

int ImagesResizedProxy::columnCount(const QModelIndex &parent) const
{
  if (this->sourceModel() == nullptr) {
    return 0;
  }

  int sourceColumns = this->sourceModel()->columnCount(parent);
  return sourceColumns + 2;
}

QVariant ImagesResizedProxy::data(const QModelIndex &index, int role) const
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
        QColor backgroundColor = Parsing::Conversion::BitmapHelper::detectBackgroundColor(&imageSource);
        QImage imageScaled = Parsing::Conversion::BitmapHelper::crop(&imageSource, this->mLeft, this->mTop, this->mRight, this->mBottom, backgroundColor);
        result = imageScaled;
      }

      break;
    }

    case Qt::SizeHintRole: {
      if (columnIndex == 1) {
        QSize size = result.toSize();
        result = this->resized(size);
      }

      break;
    }

    case Qt::DisplayRole: {
      if (columnIndex == sourceColumns + 0) {
        QVariant varSize = this->sourceModel()->data(index, Data::Models::ImagesModel::ImageSizeRole);
        QSize size = varSize.toSize();
        result = QString("%1x%2").arg(size.width()).arg(size.height());
      } else if (columnIndex == sourceColumns + 1) {
        QVariant varSize = this->sourceModel()->data(index, Data::Models::ImagesModel::ImageSizeRole);
        QSize size = varSize.toSize();
        size = this->resized(size);
        result = QString("%1x%2").arg(size.width()).arg(size.height());
      }

      break;
    }

    default:
      break;
  }

  return result;
}

QModelIndex ImagesResizedProxy::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return this->createIndex(row, column);
}

QModelIndex ImagesResizedProxy::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
  return QModelIndex();
}

QModelIndex ImagesResizedProxy::mapFromSource(const QModelIndex &sourceIndex) const
{
  return this->index(sourceIndex.row(), sourceIndex.column(), sourceIndex.parent());
}

QModelIndex ImagesResizedProxy::mapToSource(const QModelIndex &proxyIndex) const
{
  if (sourceModel() && proxyIndex.isValid()) {
    return sourceModel()->index(proxyIndex.row(), proxyIndex.column(), proxyIndex.parent());
  } else {
    return QModelIndex();
  }
}

void ImagesResizedProxy::setCrop(int left, int top, int right, int bottom)
{
  emit this->beginResetModel();

  this->mLeft = left;
  this->mTop = top;
  this->mRight = right;
  this->mBottom = bottom;

  emit this->endResetModel();
}

const QSize ImagesResizedProxy::resized(const QSize &value) const
{
  QSize result = value;
  result.rwidth() += this->mLeft + this->mRight;
  result.rheight() += this->mTop + this->mBottom;

  if (result.height() < 1) {
    result.setHeight(1);
  }

  if (result.width() < 1) {
    result.setWidth(1);
  }

  return result;
}

} // namespace Models
} // namespace Data
