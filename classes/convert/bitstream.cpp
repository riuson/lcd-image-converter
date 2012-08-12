/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
//-----------------------------------------------------------------------------
#include <QVector>
#include "conversionmatrixoptions.h"
#include "conversionmatrix.h"
//-----------------------------------------------------------------------------
BitStream::BitStream(ConversionMatrix *matrix, QVector<quint32> *data, int start, int count)
{
    this->mMatrix = matrix;
    this->mData = data;
    this->mStart = start;
    this->mCount = count;

    this->init();
}
//-----------------------------------------------------------------------------
void BitStream::init()
{
    this->mCurrentPixel = 0;
    this->mMaskCurrent = this->mMaskSource = this->mMatrix->options()->maskUsed();
    this->mBlockSize = ((int)this->mMatrix->options()->blockSize() + 1) << 3;
    this->mBitsReaded = 0;
}
//-----------------------------------------------------------------------------
bool BitStream::eof()
{
    return (this->mCurrentPixel >= this->mCount);
}
//-----------------------------------------------------------------------------
quint32 BitStream::next()
{
    quint32 result = 0;
    int i = this->mBlockSize - 1;
    quint64 fill = this->mMatrix->options()->maskFill();
    while (i >= 0)
    {
        result = result << 1;

        if ((fill & (0x00000001 << i)) == 0)
        {
            i--;
            continue;
        }

        if (!this->eof())
            result |= this->nextBit();
        i--;
    }
    return result;
}
//-----------------------------------------------------------------------------
bool BitStream::nextBit()
{
    if (this->mStart + this->mCurrentPixel >= this->mData->size())
        return false;

    bool result = false;

    quint32 data = this->mData->at(this->mStart + this->mCurrentPixel);
    for (int i = 0; i < 32; i++)
    {
        quint32 mask = 0x00000001 << i;
        if (this->mMaskCurrent & mask)
        {
            result = (data & mask) != 0;

            // reset processed pixel's bit in mask
            this->mMaskCurrent &= ~mask;

            this->mBitsReaded++;

            // if pixel completed
            if (this->mMaskCurrent == 0)
            {
                this->mMaskCurrent = this->mMaskSource;
                this->mCurrentPixel++;

                if (this->mStart + this->mCurrentPixel >= this->mData->size())
                    data = 0;
                else
                    data = this->mData->at(this->mStart + this->mCurrentPixel);
            }

            break;
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
int BitStream::remain()
{
    int result = 0;
    for (int i = 0; i < 32; i++)
    {
        if (this->mMaskCurrent & (0x00000001 << i))
            result++;
    }
    return result;
}
//-----------------------------------------------------------------------------
