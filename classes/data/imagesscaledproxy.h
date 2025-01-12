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

#ifndef IMAGESSCALEDPROXY_H
#define IMAGESSCALEDPROXY_H

#include <QSortFilterProxyModel>

namespace Data
{
namespace Models
{

class ImagesScaledProxy : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  explicit ImagesScaledProxy(QObject* parent = 0);
  virtual ~ImagesScaledProxy();

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

  int scale() const;

public slots:
  void setScale(int value);

private:
  int mScale;

signals:
  void scaleChanged(int value);
};

} // namespace Models
} // namespace Data

#endif // IMAGESSCALEDPROXY_H
