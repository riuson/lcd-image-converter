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

#ifndef IMAGESRESIZEDPROXY_H
#define IMAGESRESIZEDPROXY_H

#include <QSortFilterProxyModel>

class ImagesResizedProxy : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit ImagesResizedProxy(QObject *parent = 0);

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;

  void setCrop(int left, int top, int right, int bottom);

private:
  int mLeft;
  int mRight;
  int mTop;
  int mBottom;

  const QSize resized(const QSize &value) const;
};

#endif // IMAGESRESIZEDPROXY_H
