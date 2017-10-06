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

#ifndef STATUSDATA_H
#define STATUSDATA_H

#include <QObject>
#include <QVariant>
#include <QMap>
#include <QList>

class StatusData : public QObject
{
  Q_OBJECT
public:

  enum StatusType {
    ImageIndex,
    ImagesCount,
    ImageSize,
    MouseCoordinates,
    Scale
  };

  explicit StatusData(QObject *parent = 0);
  virtual ~StatusData() {}

  const QList<StatusType> keys() const;
  const QVariant data(const StatusType key) const;
  void setData(const StatusType key, const QVariant &value);
  void removeData(const StatusType key);

private:
  QMap<StatusType, QVariant> mData;

signals:
  void changed();
};

#endif // STATUSDATA_H
