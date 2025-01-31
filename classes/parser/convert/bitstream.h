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

#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <QtGlobal>

template <class T> class QVector;

namespace Settings
{
namespace Presets
{
class Preset;
}
} // namespace Settings

namespace Parsing
{
namespace Conversion
{

class BitStream
{
public:
  BitStream(Settings::Presets::Preset* preset, QVector<quint32>* data, int start, int count);
  virtual ~BitStream() {}

  void init();
  bool eof() const;
  quint32 next();

private:
  Settings::Presets::Preset* mPreset;
  QVector<quint32>* mData;
  int mStart;
  int mCount;
  int mBlockSize;
  bool mSetOnesByDefault;

  int mCurrentPixel;
  int mMaskSource;
  int mMaskCurrent;
  int mBitsReaded;

  bool nextBit();
  // remaining bit count from one pixel
  int remain() const;
};

} // namespace Conversion
} // namespace Parsing

#endif // BITSTREAM_H
