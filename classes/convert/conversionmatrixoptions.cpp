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
        // 'matrixData' will be automatically expanded in method 'set'
        this->set(ConversionMatrixOptions::IndexMaskAnd, 0xffffffff);
        this->set(ConversionMatrixOptions::IndexMaskFill, 0xffffffff);
    }
}
//-----------------------------------------------------------------------------
ConversionMatrixOptions::~ConversionMatrixOptions()
{
}
//-----------------------------------------------------------------------------
BytesOrder ConversionMatrixOptions::bytesOrder() const
{
    if (this->get(ConversionMatrixOptions::IndexBytesOrder) <= BytesOrderBigEndian)
    {
        return (BytesOrder)this->get(ConversionMatrixOptions::IndexBytesOrder);
    }
    return BytesOrderBigEndian;
}
//-----------------------------------------------------------------------------
ConversionType ConversionMatrixOptions::convType() const
{
    if (this->get(ConversionMatrixOptions::IndexConversionType) <= ConversionTypeColor)
    {
        return (ConversionType)this->get(ConversionMatrixOptions::IndexConversionType);
    }
    return ConversionTypeColor;
}
//-----------------------------------------------------------------------------
MonochromeType ConversionMatrixOptions::monoType() const
{
    if (this->get(ConversionMatrixOptions::IndexMonochromeType) <= MonochromeTypeThresholdDither)
    {
        return (MonochromeType)this->get(ConversionMatrixOptions::IndexMonochromeType);
    }
    return MonochromeTypeThresholdDither;
}
//-----------------------------------------------------------------------------
int ConversionMatrixOptions::edge() const
{
    if (this->get(ConversionMatrixOptions::IndexEdge) <= 255)
    {
        return (int)this->get(ConversionMatrixOptions::IndexEdge);
    }
    return 128;
}
//-----------------------------------------------------------------------------
DataBlockSize ConversionMatrixOptions::blockSize() const
{
    if (this->get(ConversionMatrixOptions::IndexDataBlockSize) <= Data32)
    {
        return (DataBlockSize)this->get(ConversionMatrixOptions::IndexDataBlockSize);
    }
    return Data32;
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskUsed() const
{
    return this->get(ConversionMatrixOptions::IndexMaskUsed);
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskAnd() const
{
    return this->get(ConversionMatrixOptions::IndexMaskAnd);
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskOr() const
{
    return this->get(ConversionMatrixOptions::IndexMaskOr);
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::maskFill() const
{
    return this->get(ConversionMatrixOptions::IndexMaskFill);
}
//-----------------------------------------------------------------------------
Rotate ConversionMatrixOptions::rotate() const
{
    if (this->get(ConversionMatrixOptions::IndexPreprocessRotation) <= Rotate270)
    {
        return (Rotate)this->get(ConversionMatrixOptions::IndexPreprocessRotation);
    }
    return Rotate270;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::flipVertical() const
{
    if (this->get(ConversionMatrixOptions::IndexPreprocessFlipVertical) != 0)
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::flipHorizontal() const
{
    if (this->get(ConversionMatrixOptions::IndexPreprocessFlipHorizontal) != 0)
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::inverse() const
{
    if (this->get(ConversionMatrixOptions::IndexPreprocessInverse) != 0)
    {
        return true;
    }
    return false;
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBytesOrder(BytesOrder value)
{
    if (value < BytesOrderLittleEndian || value > BytesOrderBigEndian)
        value = BytesOrderLittleEndian;
    this->set(ConversionMatrixOptions::IndexBytesOrder, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setConvType(ConversionType value)
{
    if (value < ConversionTypeMonochrome || value > ConversionTypeColor)
        value = ConversionTypeColor;
    this->set(ConversionMatrixOptions::IndexConversionType, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMonoType(MonochromeType value)
{
    if (value < MonochromeTypeEdge || value > MonochromeTypeThresholdDither)
        value = MonochromeTypeDiffuseDither;
    this->set(ConversionMatrixOptions::IndexMonochromeType, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setEdge(int value)
{
    if (value < 0 || value > 255)
        value = 128;
    this->set(ConversionMatrixOptions::IndexEdge, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBlockSize(DataBlockSize value)
{
    if (value < Data8 || value > Data32)
        value = Data32;
    this->set(ConversionMatrixOptions::IndexDataBlockSize, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskUsed(quint32 value)
{
    this->set(ConversionMatrixOptions::IndexMaskUsed, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskAnd(quint32 value)
{
    this->set(ConversionMatrixOptions::IndexMaskAnd, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskOr(quint32 value)
{
    this->set(ConversionMatrixOptions::IndexMaskOr, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMaskFill(quint32 value)
{
    switch (this->blockSize())
    {
    case Data8:
        if ((value & 0x000000ff) == 0)
            value = 0x000000ff;
        break;
    case Data16:
        if ((value & 0x0000ffff) == 0)
            value = 0x0000ffff;
        break;
    case Data24:
        if ((value & 0x00ffffff) == 0)
            value = 0x00ffffff;
        break;
    case Data32:
        if (value == 0)
            value = 0xffffffff;
        break;
    }

    this->set(ConversionMatrixOptions::IndexMaskFill, value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setRotate(Rotate value)
{
    if (value < RotateNone || value > Rotate270)
        value = RotateNone;
    this->set(ConversionMatrixOptions::IndexPreprocessRotation, (quint32)value);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setFlipVertical(bool value)
{
    if (value)
        this->set(ConversionMatrixOptions::IndexPreprocessFlipVertical, (quint32)1);
    else
        this->set(ConversionMatrixOptions::IndexPreprocessFlipVertical, (quint32)0);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setFlipHorizontal(bool value)
{
    if (value)
        this->set(ConversionMatrixOptions::IndexPreprocessFlipHorizontal, (quint32)1);
    else
        this->set(ConversionMatrixOptions::IndexPreprocessFlipHorizontal, (quint32)0);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setInverse(bool value)
{
    if (value)
        this->set(ConversionMatrixOptions::IndexPreprocessInverse, (quint32)1);
    else
        this->set(ConversionMatrixOptions::IndexPreprocessInverse, (quint32)0);

    emit this->changed();
}
//-----------------------------------------------------------------------------
const QString & ConversionMatrixOptions::convTypeName() const
{
    static const QString names[] =
    {
        "Monochrome",
        "Grayscale",
        "Color",
        "???"
    };
    switch (this->convType())
    {
    case ConversionTypeMonochrome:
        return names[0];
    case ConversionTypeGrayscale:
        return names[1];
    case ConversionTypeColor:
        return names[2];
    default:
        return names[3];
    }
}
//-----------------------------------------------------------------------------
const QString & ConversionMatrixOptions::monoTypeName() const
{
    static const QString names[] =
    {
        "Edge",
        "Diffuse Dither",
        "Ordered Dither",
        "Threshold Dither",
        "???"
    };
    switch (this->monoType())
    {
    case MonochromeTypeEdge:
        return names[0];
    case MonochromeTypeDiffuseDither:
        return names[1];
    case MonochromeTypeOrderedDither:
        return names[2];
    case MonochromeTypeThresholdDither:
        return names[3];
    default:
        return names[4];
    }
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::get(int index) const
{
    quint32 result = 0;
    if (index < ConversionMatrixOptions::IndexesCount)
    {
        // if not enough data
        if (this->mMatrixData->count() < ConversionMatrixOptions::IndexesCount)
        {
            for (int i = this->mMatrixData->count(); i < ConversionMatrixOptions::IndexesCount; i++)
            {
                this->mMatrixData->append(0);
            }
        }
        result = this->mMatrixData->at(index);
    }
    return result;
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::set(int index, quint32 value)
{
    if (index < ConversionMatrixOptions::IndexesCount)
    {
        // if not enough data
        if (this->mMatrixData->count() < ConversionMatrixOptions::IndexesCount)
        {
            for (int i = this->mMatrixData->count(); i < ConversionMatrixOptions::IndexesCount; i++)
            {
                this->mMatrixData->append(0);
            }
        }
        this->mMatrixData->replace(index, value);
    }
}
//-----------------------------------------------------------------------------

