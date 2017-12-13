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
#include "bitmaphelper.h"

namespace Data
{
namespace Models
{

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
  QVariant result = this->sourceModel()->data(index, role);

  if (!index.isValid()) {
    return result;
  }

  int columnIndex = index.column();

  switch (role) {
    case Qt::DecorationRole: {
      if (columnIndex == 1) {
        QImage imageSource = result.value<QImage>();
        QImage imageScaled = Parsing::Conversion::BitmapHelper::scale(&imageSource, this->mScale);
        imageScaled = Parsing::Conversion::BitmapHelper::drawGrid(&imageScaled, this->mScale);
        result = imageScaled;
      }

      break;
    }

    case Qt::SizeHintRole: {
      if (columnIndex == 1) {
        QSize size = result.toSize();
        size.scale(size.width() * this->mScale, size.height() * this->mScale, Qt::KeepAspectRatio);
        result = size;
      }

      break;
    }

    default:
      break;
  }

  return result;
}

int ImagesScaledProxy::scale() const
{
  return this->mScale;
}

void ImagesScaledProxy::setScale(int value)
{
  if (value >= 1) {
    emit this->beginResetModel();
    this->mScale = value;
    emit this->endResetModel();
    emit this->scaleChanged(this->mScale);
  }
}

} // namespace Models
} // namespace Data
