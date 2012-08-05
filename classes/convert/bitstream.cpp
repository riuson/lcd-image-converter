#include "bitstream.h"
//-----------------------------------------------------------------------------
#include "conversionmatrixoptions.h"
#include <QList>
//-----------------------------------------------------------------------------
BitStream::BitStream(QList<quint32> *matrix, QList<quint32> *data, int start, int count)
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
    ConversionMatrixOptions options(this->mMatrix);
    this->mCurrentPixel = 0;
    this->mMaskCurrent = this->mMaskSource = options.mask();
    this->mBlockSize = ((int)options.blockSize() + 1) << 3;
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
    while (i >= 0)
    {
        result = result << 1;
        if (!this->eof())
            result |= this->nextBit();
        i--;
    }
    return result;
}
//-----------------------------------------------------------------------------
bool BitStream::nextBit()
{
    bool result = false;
    for (int i = 0; i < 32; i++)
    {
        quint32 mask = 0x00000001 << i;
        if (this->mMaskCurrent & mask)
        {
            quint32 data = this->mData->at(this->mStart + this->mCurrentPixel);
            result = (data & mask) != 0;

            // reset processed pixel's bit in mask
            this->mMaskCurrent &= ~mask;

            this->mBitsReaded++;

            // if pixel completed
            if (this->mMaskCurrent == 0)
            {
                this->mMaskCurrent = this->mMaskSource;
                this->mCurrentPixel++;
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
