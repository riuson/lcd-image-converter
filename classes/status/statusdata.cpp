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

#include "statusdata.h"

namespace AppUI
{
namespace Status
{

StatusData::StatusData(QObject *parent) :
  QObject(parent)
{
}

const QList<StatusData::StatusType> StatusData::keys() const
{
  return this->mData.keys();
}

const QVariant StatusData::data(const StatusType key) const
{
  return this->mData.value(key, QString());
}

void StatusData::setData(const StatusType key, const QVariant &value)
{
  this->mData.insert(key, value);
  emit this->changed();
}

void StatusData::removeData(const StatusData::StatusType key)
{
  this->mData.remove(key);
  emit this->changed();
}

} // namespace Status
} // namespace AppUI
