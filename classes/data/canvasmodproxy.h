/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2019 riuson
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

#ifndef CANVASMODPROXY_H
#define CANVASMODPROXY_H

#include <QSortFilterProxyModel>

template <class Key, class Value> class QMap;

namespace Data
{
class CanvasModInfo;

namespace Models
{

class CanvasModProxy : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit CanvasModProxy(QMap<QString, Data::CanvasModInfo*>* canvasMods, QObject* parent = nullptr);
  virtual ~CanvasModProxy() Q_DECL_OVERRIDE {}

  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
  QModelIndex parent(const QModelIndex& index) const Q_DECL_OVERRIDE;
  QModelIndex mapFromSource(const QModelIndex& sourceIndex) const Q_DECL_OVERRIDE;
  QModelIndex mapToSource(const QModelIndex& proxyIndex) const Q_DECL_OVERRIDE;

private:
  QMap<QString, Data::CanvasModInfo*>* mCanvasMods;
};

} // namespace Models
} // namespace Data

#endif // CANVASMODPROXY_H
