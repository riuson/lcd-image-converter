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

#ifndef CONVERTERHELPER_H
#define CONVERTERHELPER_H
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
enum DataBlockSize
{
    Data8  = 0,
    Data16 = 1,
    Data24 = 2,
    Data32 = 3
};
//-----------------------------------------------------------------------------
class ConverterHelper
{
public:

    static void packDataPreview(QStringList *list, QStringList &colors, int bits, bool pack, bool alignToHigh);

    static void createMatrixMono(QList<quint32> *matrix, bool pack = true, MonochromeType type = MonochromeTypeDiffuseDither, int edge = 0x80);
    static void createMatrixGrayscale(QList<quint32> *matrix, bool pack = true, int bits = 8);
    static void createMatrixColor(QList<quint32> *matrix, bool pack = true, int redBits = 8, int greenBits = 8, int blueBits = 8);

    // gets RGB array of pixels
    // also convert images to monochrome or grayscale (r = g = b = qGray(pixel))
    static void pixelsData(QList<quint32> *matrix, QImage *image, QList<quint32> *data, int *width, int *height);
    // apply masks and bits reorder
    static void processPixels(QList<quint32> *matrix, QList<quint32> *data);
private:
    static const quint32 MaskPackData       = 0x80000000;
    static const quint32 MaskByteOrder      = 0x40000000;
    static const quint32 MaskConversionType = 0x30000000;
    static const quint32 MaskMonochromeType = 0x0f000000;
    static const quint32 MaskEdgeValue      = 0x00ff0000;
    static const quint32 MaskDataBlockSize  = 0x0000f000;

    static void makeMonochrome(QImage &image, int edge);
    static void makeGrayscale(QImage &image);
};
/*
 *  Matrix structure:
 *
 * [0] ConversionOptions:
 *     31    - pack data
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
//-----------------------------------------------------------------------------
#endif // CONVERTERHELPER_H
