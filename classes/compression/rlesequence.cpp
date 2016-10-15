/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
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

#include "rlesequence.h"

#include <QVector>

RleSequence::RleSequence()
{
  this->mData = new QVector<quint32>();
}

RleSequence::RleSequence(const RleSequence *other)
{
  this->mData = new QVector<quint32>(*other->mData);
}

RleSequence::~RleSequence()
{
  delete this->mData;
}

void RleSequence::append(quint32 value)
{
  this->mData->append(value);
}

void RleSequence::append(const RleSequence *sequence)
{
  for (quint32 i = 0; i < sequence->size(); i++) {
    this->mData->append(sequence->at(i));
  }
}

quint32 RleSequence::at(quint32 index) const
{
  return this->mData->at(index);
}

quint32 RleSequence::last() const
{
  return this->mData->last();
}

quint32 RleSequence::size() const
{
  return this->mData->size();
}

bool RleSequence::allEquals() const
{
  bool result = true;
  quint32 first = this->mData->at(0);

  for (int i = 1; i < this->mData->size(); i++) {
    if (this->mData->at(i) != first) {
      result = false;
      break;
    }
  }

  return result;
}

