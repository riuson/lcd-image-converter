#include "testconversionmatrixoptions.h"
//-----------------------------------------------------------------------------
TestConversionMatrixOptions::TestConversionMatrixOptions(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::initTestCase()
{
    this->mMatrixData = new QVector<quint32>();
    this->mOptions = new ConversionMatrixOptions(this->mMatrixData, this);
    if (this->mMatrixData->count() != 13)
        QFAIL("Matrix options not initialized");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::bytesOrder()
{
    // 1
    this->mMatrixData->replace(0, BytesOrderBigEndian);
    if (this->mOptions->bytesOrder() != BytesOrderBigEndian)
        QFAIL("Big-Endian failed");

    // 2
    this->mMatrixData->replace(0, BytesOrderLittleEndian);
    if (this->mOptions->bytesOrder() != BytesOrderLittleEndian)
        QFAIL("Little-Endian failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setBytesOrder()
{
    // 1
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    if ((BytesOrder)this->mMatrixData->at(0) != BytesOrderBigEndian)
        QFAIL("Big-Endian failed");

    // 2
    this->mOptions->setBytesOrder(BytesOrderLittleEndian);
    if ((BytesOrder)this->mMatrixData->at(0) != BytesOrderLittleEndian)
        QFAIL("Little-Endian failed");

    // 3
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    if ((BytesOrder)this->mMatrixData->at(0) != BytesOrderBigEndian)
        QFAIL("Big-Endian failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::convType()
{
    // 1
    this->mMatrixData->replace(1, ConversionTypeMonochrome);
    if (this->mOptions->convType() != ConversionTypeMonochrome)
        QFAIL("Monochrome failed");

    // 2
    this->mMatrixData->replace(1, ConversionTypeGrayscale);
    if (this->mOptions->convType() != ConversionTypeGrayscale)
        QFAIL("Grayscale failed");

    // 3
    this->mMatrixData->replace(1, ConversionTypeColor);
    if (this->mOptions->convType() != ConversionTypeColor)
        QFAIL("Color failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setConvType()
{
    // 1
    this->mOptions->setConvType(ConversionTypeMonochrome);
    if ((ConversionType)this->mMatrixData->at(1) != ConversionTypeMonochrome)
        QFAIL("Monochrome failed");

    // 2
    this->mOptions->setConvType(ConversionTypeGrayscale);
    if ((ConversionType)this->mMatrixData->at(1) != ConversionTypeGrayscale)
        QFAIL("Grayscale failed");

    // 3
    this->mOptions->setConvType(ConversionTypeColor);
    if ((ConversionType)this->mMatrixData->at(1) != ConversionTypeColor)
        QFAIL("Color failed");

    // 4
    this->mOptions->setConvType(ConversionTypeMonochrome);
    if ((ConversionType)this->mMatrixData->at(1) != ConversionTypeMonochrome)
        QFAIL("Monochrome failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::monoType()
{
    // 1
    this->mMatrixData->replace(2, MonochromeTypeEdge);
    if (this->mOptions->monoType() != MonochromeTypeEdge)
        QFAIL("Edge failed");

    // 2
    this->mMatrixData->replace(2, MonochromeTypeDiffuseDither);
    if (this->mOptions->monoType() != MonochromeTypeDiffuseDither)
        QFAIL("Diffuse Dither failed");

    // 3
    this->mMatrixData->replace(2, MonochromeTypeOrderedDither);
    if (this->mOptions->monoType() != MonochromeTypeOrderedDither)
        QFAIL("Ordered Dither failed");

    // 4
    this->mMatrixData->replace(2, MonochromeTypeThresholdDither);
    if (this->mOptions->monoType() != MonochromeTypeThresholdDither)
        QFAIL("Threshold Dither failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setMonoType()
{
    // 1
    this->mOptions->setMonoType(MonochromeTypeEdge);
    if ((MonochromeType)this->mMatrixData->at(2) != MonochromeTypeEdge)
        QFAIL("Edge failed");

    // 2
    this->mOptions->setMonoType(MonochromeTypeDiffuseDither);
    if ((MonochromeType)this->mMatrixData->at(2) != MonochromeTypeDiffuseDither)
        QFAIL("Diffuse Dither failed");

    // 3
    this->mOptions->setMonoType(MonochromeTypeOrderedDither);
    if ((MonochromeType)this->mMatrixData->at(2) != MonochromeTypeOrderedDither)
        QFAIL("Ordered Dither failed");

    // 4
    this->mOptions->setMonoType(MonochromeTypeThresholdDither);
    if ((MonochromeType)this->mMatrixData->at(2) != MonochromeTypeThresholdDither)
        QFAIL("Threshold Dither failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::edge()
{
    for (int i = -512; i < 512; i++)
    {
        this->mMatrixData->replace(3, (quint32)i);
        if (i >= 0 && i <= 255)
        {
            if (this->mOptions->edge() != i)
                QFAIL("Edge not matched in range");
        }
        else
        {
            if (this->mOptions->edge() < 0 || this->mOptions->edge() > 255)
                    QFAIL("Edge out of range");
        }
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setEdge()
{
    for (int i = -512; i < 512; i++)
    {
        this->mOptions->setEdge(i);
        if (i >= 0 && i <= 255)
        {
            if (this->mMatrixData->at(3) != i)
                QFAIL("Edge not matched in range");
        }
        else
        {
            if (this->mMatrixData->at(3) > 255)
                    QFAIL("Edge out of range");
        }
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::blockSize()
{
    // 1
    this->mMatrixData->replace(4, Data8);
    if (this->mOptions->blockSize() != Data8)
        QFAIL("Data8 failed");

    // 2
    this->mMatrixData->replace(4, Data16);
    if (this->mOptions->blockSize() != Data16)
        QFAIL("Data16 failed");

    // 3
    this->mMatrixData->replace(4, Data24);
    if (this->mOptions->blockSize() != Data24)
        QFAIL("Data24 failed");

    // 4
    this->mMatrixData->replace(4, Data32);
    if (this->mOptions->blockSize() != Data32)
        QFAIL("Data32 failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setBlockSize()
{
    // 1
    this->mOptions->setBlockSize(Data8);
    if ((DataBlockSize)this->mMatrixData->at(4) != Data8)
        QFAIL("Data8 failed");

    // 2
    this->mOptions->setBlockSize(Data16);
    if ((DataBlockSize)this->mMatrixData->at(4) != Data16)
        QFAIL("Data16 failed");

    // 3
    this->mOptions->setBlockSize(Data24);
    if ((DataBlockSize)this->mMatrixData->at(4) != Data24)
        QFAIL("Data24 failed");

    // 4
    this->mOptions->setBlockSize(Data32);
    if ((DataBlockSize)this->mMatrixData->at(4) != Data32)
        QFAIL("Data32 failed");

    // 5
    this->mOptions->setBlockSize(Data8);
    if ((DataBlockSize)this->mMatrixData->at(4) != Data8)
        QFAIL("Data8 failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::rotate()
{
    // 1
    this->mMatrixData->replace(5, RotateNone);
    if (this->mOptions->rotate() != RotateNone)
        QFAIL("Rotate 0 failed");

    // 2
    this->mMatrixData->replace(5, Rotate90);
    if (this->mOptions->rotate() != Rotate90)
        QFAIL("Rotate 90 failed");

    // 3
    this->mMatrixData->replace(5, Rotate180);
    if (this->mOptions->rotate() != Rotate180)
        QFAIL("Rotate 180 failed");

    // 4
    this->mMatrixData->replace(5, Rotate270);
    if (this->mOptions->rotate() != Rotate270)
        QFAIL("Rotate 270 failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setRotate()
{
    // 1
    this->mOptions->setRotate(RotateNone);
    if (this->mMatrixData->at(5) != RotateNone)
        QFAIL("Rotate 0 failed");

    // 2
    this->mOptions->setRotate(Rotate90);
    if (this->mMatrixData->at(5) != Rotate90)
        QFAIL("Rotate 90 failed");

    // 3
    this->mOptions->setRotate(Rotate180);
    if (this->mMatrixData->at(5) != Rotate180)
        QFAIL("Rotate 180 failed");

    // 4
    this->mOptions->setRotate(Rotate270);
    if (this->mMatrixData->at(5) != Rotate270)
        QFAIL("Rotate 270 failed");

    // 5
    this->mOptions->setRotate(RotateNone);
    if (this->mMatrixData->at(5) != RotateNone)
        QFAIL("Rotate 0 failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::flipVertical()
{
    // 1
    this->mMatrixData->replace(6, 0);
    if (this->mOptions->flipVertical() != false)
        QFAIL("Flip vertical failed");

    // 2
    this->mMatrixData->replace(6, 1);
    if (this->mOptions->flipVertical() != true)
        QFAIL("Flip vertical failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setFlipVertical()
{
    // 1
    this->mOptions->setFlipVertical(false);
    if (this->mMatrixData->at(6) != 0)
        QFAIL("Flip vertical failed");

    // 2
    this->mOptions->setFlipVertical(true);
    if (this->mMatrixData->at(6) != 1)
        QFAIL("Flip vertical failed");

    // 3
    this->mOptions->setFlipVertical(false);
    if (this->mMatrixData->at(6) != 0)
        QFAIL("Flip vertical failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::flipHorizontal()
{
    // 1
    this->mMatrixData->replace(7, 0);
    if (this->mOptions->flipHorizontal() != false)
        QFAIL("Flip horizontal failed");

    // 2
    this->mMatrixData->replace(7, 1);
    if (this->mOptions->flipHorizontal() != true)
        QFAIL("Flip horizontal failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setFlipHorizontal()
{
    // 1
    this->mOptions->setFlipHorizontal(false);
    if (this->mMatrixData->at(7) != 0)
        QFAIL("Flip horizontal failed");

    // 2
    this->mOptions->setFlipHorizontal(true);
    if (this->mMatrixData->at(7) != 1)
        QFAIL("Flip horizontal failed");

    // 3
    this->mOptions->setFlipHorizontal(false);
    if (this->mMatrixData->at(7) != 0)
        QFAIL("Flip horizontal failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::inverse()
{
    // 1
    this->mMatrixData->replace(8, 0);
    if (this->mOptions->inverse() != false)
        QFAIL("Inverse failed");

    // 2
    this->mMatrixData->replace(8, 1);
    if (this->mOptions->inverse() != true)
        QFAIL("Inverse failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setInverse()
{
    // 1
    this->mOptions->setInverse(false);
    if (this->mMatrixData->at(8) != 0)
        QFAIL("Inverse failed");

    // 2
    this->mOptions->setInverse(true);
    if (this->mMatrixData->at(8) != 1)
        QFAIL("Inverse failed");

    // 3
    this->mOptions->setInverse(false);
    if (this->mMatrixData->at(8) != 0)
        QFAIL("Inverse failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::mask()
{
    quint32 value1 = 0x12345678;
    quint32 value2 = 0x23456789;
    quint32 value3 = 0x57890155;
    quint32 value4 = 0x4687489a;

    for (int i = 0; i < 4; i++)
    {
        this->mMatrixData->replace(9, value1);
        if (this->mOptions->maskUsed() != value1)
            QFAIL("MaskUsed failed");

        this->mMatrixData->replace(10, value2);
        if (this->mOptions->maskAnd() != value2)
            QFAIL("MaskAnd failed");

        this->mMatrixData->replace(11, value3);
        if (this->mOptions->maskOr() != value3)
            QFAIL("MaskOr failed");

        this->mMatrixData->replace(12, value4);
        if (this->mOptions->maskFill() != value4)
            QFAIL("MaskFill failed");

        value1 = value1 >> 1;
        value2 = value2 >> 2;
        value3 = value3 >> 3;
        value4 = value4 >> 4;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setMask()
{
    quint32 value1 = 0x12345678;
    quint32 value2 = 0x23456789;
    quint32 value3 = 0x57890155;
    quint32 value4 = 0x4687489a;

    for (int i = 0; i < 4; i++)
    {
        this->mOptions->setMaskUsed(value1);
        if (this->mMatrixData->at(9) != value1)
            QFAIL("MaskUsed failed");

        this->mOptions->setMaskAnd(value2);
        if (this->mMatrixData->at(10) != value2)
            QFAIL("MaskAnd failed");

        this->mOptions->setMaskOr(value3);
        if (this->mMatrixData->at(11) != value3)
            QFAIL("MaskOr failed");

        this->mOptions->setMaskFill(value4);
        if (this->mMatrixData->at(12) != value4)
            QFAIL("MaskFill failed");

        value1 = value1 >> 1;
        value2 = value2 >> 2;
        value3 = value3 >> 3;
        value4 = value4 >> 4;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::cleanupTestCase()
{
    delete this->mOptions;
    delete this->mMatrixData;
}
//-----------------------------------------------------------------------------
