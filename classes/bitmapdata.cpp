/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#include "bitmapdata.h"
//-----------------------------------------------------------------------------
BitmapData::BitmapData()
{
}
//-----------------------------------------------------------------------------
int BitmapData::width() const
{
    return this->mWidth;
}
//-----------------------------------------------------------------------------
void BitmapData::setWidth(int value)
{
    this->mWidth = value;
}
//-----------------------------------------------------------------------------
int BitmapData::height() const
{
    return this->mHeight;
}
//-----------------------------------------------------------------------------
void BitmapData::setHeight(int value)
{
    this->mHeight = value;
}
//-----------------------------------------------------------------------------
int BitmapData::blockSize() const
{
    return this->mBlockSize;
}
//-----------------------------------------------------------------------------
void BitmapData::setBlockSize(int value)
{
    this->mBlockSize = value;
}
//-----------------------------------------------------------------------------
int BitmapData::blocksPerLine() const
{
    return this->mData.count() / this->mHeight;
}
//-----------------------------------------------------------------------------
int BitmapData::blocksCount()
{
    return this->mData.count();
}
//-----------------------------------------------------------------------------
void BitmapData::addBlock(quint32 value)
{
    this->mData.append(value);
}
//-----------------------------------------------------------------------------
const QList <quint32> *BitmapData::data() const
{
    return &this->mData;
}
//-----------------------------------------------------------------------------
void BitmapData::clear()
{
    this->mData.clear();
}
//-----------------------------------------------------------------------------
void BitmapData::swapBytes()
{
    for (int i = 0; i < this->mData.count(); i++)
    {
        quint32 value = this->mData.at(i);
        quint32 b = value;
        if (this->mBlockSize == 32)
        {
            b = 0;
            b |= (value & 0x000000ff) << 24;
            b |= (value & 0x0000ff00) << 8;
            b |= (value & 0x00ff0000) >> 8;
            b |= (value & 0xff000000) >> 24;
        }
        if (this->mBlockSize == 16)
        {
            b = 0;
            b |= (value & 0x000000ff) << 8;
            b |= (value & 0x0000ff00) >> 8;
        }
        this->mData[i] = b;
    }
}
//-----------------------------------------------------------------------------
void BitmapData::mirrorBytes()
{
    quint32 mask = 0x80808080;
    for (int i = 0; i < this->mData.count(); i++)
    {
        quint32 value = this->mData.at(i);
        quint32 b = 0;

        b |= (value & (mask >> 0)) >> 7;
        b |= (value & (mask >> 1)) >> 5;
        b |= (value & (mask >> 2)) >> 3;
        b |= (value & (mask >> 3)) >> 1;
        b |= (value & (mask >> 4)) << 1;
        b |= (value & (mask >> 5)) << 3;
        b |= (value & (mask >> 6)) << 5;
        b |= (value & (mask >> 7)) << 7;

        this->mData[i] = b;
    }
}
//-----------------------------------------------------------------------------
