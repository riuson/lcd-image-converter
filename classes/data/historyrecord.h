/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <QImage>
#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

namespace Data
{
namespace History
{

class HistoryRecord : public QObject
{
  Q_OBJECT
public:
  explicit HistoryRecord(const QStringList* _keys, const QMap<QString, QImage*>* _images,
                         const QMap<QString, QVariant>* _info, QObject* parent = 0);
  virtual ~HistoryRecord();

  const QStringList* keys() const;
  const QMap<QString, QImage*>* images() const;
  const QMap<QString, QVariant>* info() const;

private:
  QStringList mKeys;
  QMap<QString, QImage*> mImageMap;
  QMap<QString, QVariant> mInfoMap;

signals:

public slots:
};

} // namespace History
} // namespace Data

#endif // HISTORYRECORD_H
