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

#ifndef IMAGESMODEL_H
#define IMAGESMODEL_H

#include <QAbstractItemModel>

namespace Data
{
namespace Containers
{
class DataContainer;
}

namespace Models
{

class ImagesModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  enum ImagesModelRoles { KeyRole = Qt::UserRole + 1, KeyCodeRole, ImageRole, ImageSizeRole };

  explicit ImagesModel(Containers::DataContainer* container, QObject* parent = 0);
  virtual ~ImagesModel() {}

  int rowCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex& parent) const Q_DECL_OVERRIDE;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;

  void callReset();

private:
  Containers::DataContainer* mContainer;

  QVariant containerValue(int imageIndex, ImagesModelRoles role) const;

private slots:
  void imagesChanged();
};

} // namespace Models
} // namespace Data

#endif // IMAGESMODEL_H
