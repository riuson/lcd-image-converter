#include "conversionmatrixoptions.h"

#include <QList>
//-----------------------------------------------------------------------------
ConversionMatrixOptions::ConversionMatrixOptions(QList<quint32> *matrix)
{
    this->mMatrix = matrix;
    if (matrix->length() == 0)
    {
        matrix->append(0);
        matrix->append(0);
    }
}
//-----------------------------------------------------------------------------
bool ConversionMatrixOptions::pack()
{
    return (this->mMatrix->at(0) & MaskPackData) == MaskPackData;
}
//-----------------------------------------------------------------------------
BytesOrder ConversionMatrixOptions::bytesOrder()
{
    if ((this->mMatrix->at(0) & MaskByteOrder) == MaskByteOrder)
        return BytesOrderBigEndian;
    return BytesOrderLittleEndian;
}
//-----------------------------------------------------------------------------
ConversionType ConversionMatrixOptions::convType()
{
    ConversionType result = (ConversionType)((this->mMatrix->at(0) & MaskConversionType) >> 28);
    return result;
}
//-----------------------------------------------------------------------------
MonochromeType ConversionMatrixOptions::monoType()
{
    MonochromeType result = (MonochromeType)((this->mMatrix->at(0) & MaskMonochromeType) >> 24);
    return result;
}
//-----------------------------------------------------------------------------
int ConversionMatrixOptions::edge()
{
    int result = (this->mMatrix->at(0) & MaskEdgeValue) >> 16;
    return result;
}
//-----------------------------------------------------------------------------
DataBlockSize ConversionMatrixOptions::blockSize()
{
    DataBlockSize result = (DataBlockSize)((this->mMatrix->at(0) & MaskDataBlockSize) >> 12);
    return result;
}
//-----------------------------------------------------------------------------
quint32 ConversionMatrixOptions::mask()
{
    return this->mMatrix->at(1);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setPack(bool value)
{
    quint32 result = this->mMatrix->at(0);
    if (value)
        result |= MaskPackData;
    else
        result &= ~MaskPackData;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBytesOrder(BytesOrder value)
{
    quint32 result = this->mMatrix->at(0);
    if (value == BytesOrderBigEndian)
        result |= MaskByteOrder;
    else
        result &= ~MaskByteOrder;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setConvType(ConversionType value)
{
    quint32 result = this->mMatrix->at(0);
    quint32 mask = value;
    mask &= 0x00000003;
    result &= MaskConversionType;
    result |= mask << 28;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMonoType(MonochromeType value)
{
    quint32 result = this->mMatrix->at(0);
    quint32 mask = value;
    mask &= 0x0000000f;
    result &= MaskMonochromeType;
    result |= mask << 24;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setEdge(int value)
{
    quint32 result = this->mMatrix->at(0);
    quint32 mask = value & 0x000000ff;
    result &= MaskEdgeValue;
    result |= mask << 16;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setBlockSize(DataBlockSize value)
{
    quint32 result = this->mMatrix->at(0);
    quint32 mask = value;
    mask &= 0x0000000f;
    result &= MaskDataBlockSize;
    result |= mask << 12;
    this->mMatrix->replace(0, result);
}
//-----------------------------------------------------------------------------
void ConversionMatrixOptions::setMask(quint32 value)
{
    this->mMatrix->replace(1, value);
}
//-----------------------------------------------------------------------------
