#ifndef BITSTREAM_H
#define BITSTREAM_H
//-----------------------------------------------------------------------------
#include <QtGlobal>
//-----------------------------------------------------------------------------
template <class T> class QList;
//-----------------------------------------------------------------------------
class BitStream
{
public:
    BitStream(QList<quint32> *matrix, QList<quint32> *data, int start, int count);

    void init();
    bool eof();
    quint32 next();

private:
    QList<quint32> *mMatrix;
    QList<quint32> *mData;
    int mStart;
    int mCount;
    int mBlockSize;

    int mCurrentPixel;
    int mMaskSource;
    int mMaskCurrent;
    int mBitsReaded;

    bool nextBit();
    // remaining bit count from one pixel
    int remain();
};
//-----------------------------------------------------------------------------
#endif // BITSTREAM_H
