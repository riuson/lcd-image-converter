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

#ifndef RLESEQUENCE_H
#define RLESEQUENCE_H

#include <QObject>

template <class T> class QVector;

namespace Utils
{
namespace Compression
{

class RleSequence
{
public:
  RleSequence();
  RleSequence(const RleSequence *other);
  virtual ~RleSequence();

  void append(quint32 value);
  void append(const RleSequence *sequence);
  quint32 at(quint32 index) const;
  quint32 last() const;
  quint32 size() const;

  bool allEquals() const;

private:
  QVector <quint32> *mData;

};

} // namespace Compression
} // namespace Utils

#endif // RLESEQUENCE_H
