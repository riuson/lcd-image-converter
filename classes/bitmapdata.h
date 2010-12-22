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
