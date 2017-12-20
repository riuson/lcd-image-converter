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

#ifndef RLE_H
#define RLE_H

#include <QObject>

#include "conversion_options.h"

// Run-length encoding algorithm
// http://en.wikipedia.org/wiki/Run-length_encoding

template <class T> class QVector;
template <class T> class QQueue;

namespace Utils
{
namespace Compression
{

class RleSequence;

class RleCompressor : public QObject
{
  Q_OBJECT
public:
  explicit RleCompressor(QObject *parent = 0);
  virtual ~RleCompressor() {}

  void compress(
    QVector<quint32> *input,
    Parsing::Conversion::Options::DataBlockSize dataSize,
    QVector<quint32> *output,
    quint32 minimumOfEquals = 2);

private:
  void collectSequences(
    const QVector<quint32> *input,
    QVector<RleSequence *> *sequences);
  void combineSequences(
    const QVector<RleSequence *> *inputSequences,
    quint32 minimumOfEquals,
    QVector<RleSequence *> *outputSequences);
  quint32 getMaxSize(Parsing::Conversion::Options::DataBlockSize dataSize);
  void flushSequence(
    const RleSequence *sequence,
    Parsing::Conversion::Options::DataBlockSize dataSize,
    QVector<quint32> *output);
  void flushSequencePart(
    const RleSequence *sequence,
    quint32 start,
    quint32 length,
    QVector<quint32> *output);
};

} // namespace Compression
} // namespace Utils

#endif // RLE_H
