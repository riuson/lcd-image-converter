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

#ifndef COLUMNSREORDERPROXY_H
#define COLUMNSREORDERPROXY_H

#include <QSortFilterProxyModel>
#include <QList>

class ColumnsReorderProxy : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit ColumnsReorderProxy(QObject *parent = 0);
  virtual ~ColumnsReorderProxy() {}

  /*
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  */
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
  QModelIndex mapFromSource(const QModelIndex &sourceIndex) const Q_DECL_OVERRIDE;
  QModelIndex mapToSource(const QModelIndex &proxyIndex) const Q_DECL_OVERRIDE;

  void setReorder(int oldPosition, int newPosition);
private:
  int mFromColumn;
  int mToColumn;
  QList<int> mListFromSource;
  QList<int> mListToSource;

  int columnFromSource(int value) const;
  int columnToSource(int value) const;
};

#endif // COLUMNSREORDERPROXY_H
