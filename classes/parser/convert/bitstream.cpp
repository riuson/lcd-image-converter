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

#include "bitstream.h"

#include <QVector>
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"

namespace Parsing
{
namespace Conversion
{

BitStream::BitStream(Preset *preset, QVector<quint32> *data, int start, int count)
{
  this->mPreset = preset;
  this->mData = data;
  this->mStart = start;
  this->mCount = count;

  this->init();
}

void BitStream::init()
{
  this->mCurrentPixel = 0;
  this->mMaskCurrent = this->mMaskSource = this->mPreset->matrix()->maskUsed();
  this->mBlockSize = ((int)this->mPreset->image()->blockSize() + 1) << 3;
  this->mBitsReaded = 0;
  this->mSetOnesByDefault = this->mPreset->image()->blockDefaultOnes();
}

bool BitStream::eof() const
{
  if (this->mStart + this->mCurrentPixel >= this->mData->size()) {
    return true;
  }

  if (this->mCurrentPixel >= this->mCount) {
    return true;
  }

  return false;
}

quint32 BitStream::next()
{
  quint32 result;

  if (this->mSetOnesByDefault) {
    result = 0xffffffff;
  } else {
    result = 0;
  }

  // Number of bits
  int i = this->mBlockSize - 1;
  // '1' bit - filled output bit
  quint32 fill = this->mPreset->matrix()->maskFill();

  while (i >= 0) {
    result = result << 1;

    if (this->mSetOnesByDefault) {
      result |= 0x00000001;
    }

    // Loop until filled '1' bit
    if ((fill & (0x00000001 << i)) == 0) {
      i--;
      continue;
    }

    // If bits available
    if (!this->eof()) {
      // Get next bit from stream and put to result
      if (this->nextBit()) {
        result |= 0x00000001;
      } else {
        result &= 0xfffffffe;
      }
    }

    i--;
  }

  return result;
}

bool BitStream::nextBit()
{
  if (this->eof()) {
    return false;
  }

  bool result = false;
  // Initial pixel data
  quint32 data = this->mData->at(this->mStart + this->mCurrentPixel);

  // Loop bits...
  for (int i = 0; i < 32; i++) {
    // From MSB to LSB
    quint32 mask = 0x80000000 >> i;

    // If bit is used
    if (this->mMaskCurrent & mask) {
      result = (data & mask) != 0;

      // Reset processed pixel's bit in mask
      this->mMaskCurrent &= ~mask;

      this->mBitsReaded++;

      // If pixel completed
      if (this->mMaskCurrent == 0) {
        // Reload mask
        this->mMaskCurrent = this->mMaskSource;
        // Go to next pixel in source data array
        this->mCurrentPixel++;

        if (this->eof()) {
          data = 0;
        } else {
          data = this->mData->at(this->mStart + this->mCurrentPixel);
        }
      }

      break;
    }
  }

  return result;
}

int BitStream::remain() const
{
  int result = 0;

  for (int i = 0; i < 32; i++) {
    if (this->mMaskCurrent & (0x00000001 << i)) {
      result++;
    }
  }

  return result;
}

} // namespace Conversion
} // namespace Parsing
