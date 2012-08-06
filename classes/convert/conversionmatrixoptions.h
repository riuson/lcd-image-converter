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

#ifndef CONVERSIONMATRIXOPTIONS_H
#define CONVERSIONMATRIXOPTIONS_H
//-----------------------------------------------------------------------------
#include <QtGlobal>
//-----------------------------------------------------------------------------
class QStringList;
class QImage;
template <class T> class QList;
//-----------------------------------------------------------------------------
enum ConversionType
{
    ConversionTypeMonochrome = 0,
    ConversionTypeGrayscale  = 1,
    ConversionTypeColor      = 2
};
//-----------------------------------------------------------------------------
enum MonochromeType
{
    MonochromeTypeEdge = 0,
    MonochromeTypeDiffuseDither = 1,
    MonochromeTypeOrderedDither = 2,
    MonochromeTypeThresholdDither = 3
};
//-----------------------------------------------------------------------------
enum ColorsOrder
{
    ColorsOrderRGB = 0,
    ColorsOrderRBG = 1,
    ColorsOrderGRB = 2,
    ColorsOrderGBR = 3,
    ColorsOrderBRG = 4,
    ColorsOrderBGR = 5
};
//-----------------------------------------------------------------------------
enum BytesOrder
{
    BytesOrderLittleEndian,
    BytesOrderBigEndian
};
//-----------------------------------------------------------------------------
enum DataBlockSize
{
    Data8  = 0,
    Data16 = 1,
    Data24 = 2,
    Data32 = 3
};
//-----------------------------------------------------------------------------
/*
 *  Matrix structure:
 *
 * [0] ConversionOptions:
 *     31    - not used more
 *     30    - byte order, 0 = little-endian, 1 = big-endian
 *     29:28 - conversion type
 *     27:24 - monochrome type
 *     23:16 - edge value for MonochromeTypeEdge
 *     15:12 - data block size
 *
 * [1] Mask of used bits before packing
 *
 * [2] Mask of data bit
 * [3] Shift of data bit:
 *     31    - 0 = to right, 1 = to left
 *     4:0   - count
 *
 * ... Repeat 2 and 3 some times
 */

class ConversionMatrixOptions
{
public:
    ConversionMatrixOptions(QList<quint32> *matrix);

    BytesOrder bytesOrder();
    ConversionType convType();
    MonochromeType monoType();
    int edge();
    DataBlockSize blockSize();
    quint32 mask();

    void setBytesOrder(BytesOrder value);
    void setConvType(ConversionType value);
    void setMonoType(MonochromeType value);
    void setEdge(int value);
    void setBlockSize(DataBlockSize value);
    void setMask(quint32 value);

private:
    static const quint32 MaskByteOrder      = 0x40000000;
    static const quint32 MaskConversionType = 0x30000000;
    static const quint32 MaskMonochromeType = 0x0f000000;
    static const quint32 MaskEdgeValue      = 0x00ff0000;
    static const quint32 MaskDataBlockSize  = 0x0000f000;

    QList<quint32> *mMatrix;
};
//-----------------------------------------------------------------------------
#endif // CONVERSIONMATRIXOPTIONS_H
