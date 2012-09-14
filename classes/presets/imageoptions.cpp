#include "imageoptions.h"
//-----------------------------------------------------------------------------
ImageOptions::ImageOptions(QObject *parent) :
    QObject(parent)
{
    this->mBytesOrder = BytesOrderLittleEndian;
    this->mBlockSize = Data8;
}
//-----------------------------------------------------------------------------
BytesOrder ImageOptions::bytesOrder() const
{
    return this->mBytesOrder;
}
//-----------------------------------------------------------------------------
DataBlockSize ImageOptions::blockSize() const
{
    if (this->mBlockSize <= Data32)
    {
        return this->mBlockSize;
    }
    return Data32;
}
//-----------------------------------------------------------------------------
void ImageOptions::setBytesOrder(BytesOrder value)
{
    if (value < BytesOrderLittleEndian || value > BytesOrderBigEndian)
        value = BytesOrderLittleEndian;
    this->mBytesOrder = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ImageOptions::setBlockSize(DataBlockSize value)
{
    if (value < Data8 || value > Data32)
        value = Data32;
    this->mBlockSize = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
