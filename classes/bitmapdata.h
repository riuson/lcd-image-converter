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

#ifndef BITMAPDATA_H
#define BITMAPDATA_H
//-----------------------------------------------------------------------------
#include <QList>
//-----------------------------------------------------------------------------
class BitmapData
{
public:
    BitmapData();
    int width() const;
    void setWidth(int value);
    int height() const;
    void setHeight(int value);
    int blockSize() const;
    void setBlockSize(int value);
    int blocksPerLine() const;    // number of data blocks per image line
    int blocksCount();

    void addBlock(quint32 value);
    const QList <quint32> *data() const;
    void clear();
    void swapBytes();// change between little-endian and big-endian
    void mirrorBytes();// mirror each byte of data
private:
    int mWidth;             // width of image in pixels
    int mHeight;            // height of image in pixels
    int mBlockSize;         // size of data block: 8, 16 or 32 bits
    QList <quint32> mData;  // data array
};
//-----------------------------------------------------------------------------
#endif // BITMAPDATA_H
