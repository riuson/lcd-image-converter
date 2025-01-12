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

#ifndef IMAGESFILTERPROXY_H
#define IMAGESFILTERPROXY_H

#include <QSortFilterProxyModel>

#include <QStringList>

namespace Data
{
namespace Models
{

class ImagesFilterProxy : public QSortFilterProxyModel
{
  Q_OBJECT
public:
  explicit ImagesFilterProxy(QObject *parent = 0);
  virtual ~ImagesFilterProxy();

  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

  void setFilter(const QStringList &keys);

private:
  QStringList *mKeys;

};

} // namespace Models
} // namespace Data

#endif // IMAGESFILTERPROXY_H
