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

#include "conversionmatrixoptions.h"

#include <QVector>
//-----------------------------------------------------------------------------
ConversionMatrixOptions::ConversionMatrixOptions(QVector<quint32> *matrixData, QObject *parent) :
    QObject(parent)
{
    this->mMatrixData = matrixData;
    if (matrixData->size() == 0)
    {
        matrixData->append(0);
        matrixData->append(0);
        matrixData->append(0xffffffff);
        matrixData->append(0);
    }
}
//-----------------------------------------------------------------------------
ConversionMatrixOptions::~ConversionMatrixOptions()
{
}
//-----------------------------------------------------------------------------
BytesOrder ConversionMatrixOptions::bytesOrder()
{
    if ((this->mMatrixData->at(0) & MaskByteOrder) == MaskByteOrder)
        return BytesOrderBigEndian;
    return BytesOrderLittleEndian;
}
//-----------------------------------------------------------------------------
ConversionType ConversionMatrixOptions::convType()
{
    ConversionType result = (ConversionType)((this->mMatrixData->at(0) & MaskConversionType) >> 28);
    return result;
}
//-----------------------------------------------------------------------------
MonochromeType ConversionMatrixOptions::monoType()
{
    MonochromeType result = (MonochromeType)((this->mMatrixData->at(0) & MaskMonochromeType) >> 24);
    return result;
}
//-----------------------------------------------------------------------------
int ConversionMatrixOptions::edge()
{
    int result = (this->mMatrixData->at(0) & MaskEdgeValue) >> 16;
    return result;
}
//-----------------------------------------------------------------------------
DataBlockSize ConversionMatrixOptions::blockSize()
{
    DataBlockSize result = (DataBlockSize)((this->mMatrixData->at(0) & MaskDataBlockSize) >> 12);
    return result;
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskUsed()
{
    return this->mMatrixData->at(1);
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskAnd()
{
    return this->mMatrixData->at(2);
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskOr()
{
    return this->mMatrixData->at(3);
}
//-----------------------------------------------------------------------------
Rotate ConversionMatrixOptions::rotate()
{
    Rotate result = (Rotate)((this->mMatrixData->at(0) & MaskRotate) >> 10);
    return result;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::flipVertical()
{
    return (this->mMatrixData->at(0) & MaskFlipV) != 0;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::flipHorizontal()
{
    return (this->mMatrixData->at(0) & MaskFlipH) != 0;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::inverse()
{
    return (this->mMatrixData->at(0) & MaskInverse) != 0;
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBytesOrder(BytesOrder value)
{
    quint32 result = this->mMatrixData->at(0);
    if (value == BytesOrderBigEndian)
        result |= MaskByteOrder;
    else
        result &= ~MaskByteOrder;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setConvType(ConversionType value)
{
    quint32 result = this->mMatrixData->at(0);
    quint32 mask = value;
    mask &= 0x00000003;
    result &= ~MaskConversionType;
    result |= mask << 28;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMonoType(MonochromeType value)
{
    quint32 result = this->mMatrixData->at(0);
    quint32 mask = value;
    mask &= 0x0000000f;
    result &= ~MaskMonochromeType;
    result |= mask << 24;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setEdge(int value)
{
    quint32 result = this->mMatrixData->at(0);
    quint32 mask = value & 0x000000ff;
    result &= ~MaskEdgeValue;
    result |= mask << 16;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBlockSize(DataBlockSize value)
{
    quint32 result = this->mMatrixData->at(0);
    quint32 mask = value;
    mask &= 0x0000000f;
    result &= ~MaskDataBlockSize;
    result |= mask << 12;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskUsed(quint32 value)
{
    this->mMatrixData->replace(1, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskAnd(quint32 value)
{
    this->mMatrixData->replace(2, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskOr(quint32 value)
{
    this->mMatrixData->replace(3, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setRotate(Rotate value)
{
    quint32 result = this->mMatrixData->at(0);
    quint32 mask = value;
    mask &= 0x00000003;
    result &= ~MaskRotate;
    result |= mask << 10;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setFlipVertical(bool value)
{
    quint32 result = this->mMatrixData->at(0);
    if (value)
        result |= MaskFlipV;
    else
        result &= ~MaskFlipV;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setFlipHorizontal(bool value)
{
    quint32 result = this->mMatrixData->at(0);
    if (value)
        result |= MaskFlipH;
    else
        result &= ~MaskFlipH;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setInverse(bool value)
{
    quint32 result = this->mMatrixData->at(0);
    if (value)
        result |= MaskInverse;
    else
        result &= ~MaskInverse;
    this->mMatrixData->replace(0, result);

    emit this->changed();
}
//-----------------------------------------------------------------------------
