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
    QVERIFY2(this->mMatrixData->count() == 13, "Matrix options not initialized");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::bytesOrder()
{
    // 1
    this->mMatrixData->replace(0, BytesOrderBigEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderBigEndian);

    // 2
    this->mMatrixData->replace(0, BytesOrderLittleEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderLittleEndian);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setBytesOrder()
{
    // 1
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    QCOMPARE(this->mMatrixData->at(0), (quint32)BytesOrderBigEndian);

    // 2
    this->mOptions->setBytesOrder(BytesOrderLittleEndian);
    QCOMPARE(this->mMatrixData->at(0), (quint32)BytesOrderLittleEndian);

    // 3
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    QCOMPARE(this->mMatrixData->at(0), (quint32)BytesOrderBigEndian);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::convType()
{
    // 1
    this->mMatrixData->replace(1, ConversionTypeMonochrome);
    QCOMPARE(this->mOptions->convType(), ConversionTypeMonochrome);

    // 2
    this->mMatrixData->replace(1, ConversionTypeGrayscale);
    QCOMPARE(this->mOptions->convType(), ConversionTypeGrayscale);

    // 3
    this->mMatrixData->replace(1, ConversionTypeColor);
    QCOMPARE(this->mOptions->convType(), ConversionTypeColor);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setConvType()
{
    // 1
    this->mOptions->setConvType(ConversionTypeMonochrome);
    QCOMPARE(this->mMatrixData->at(1), (quint32)ConversionTypeMonochrome);

    // 2
    this->mOptions->setConvType(ConversionTypeGrayscale);
    QCOMPARE(this->mMatrixData->at(1), (quint32)ConversionTypeGrayscale);

    // 3
    this->mOptions->setConvType(ConversionTypeColor);
    QCOMPARE(this->mMatrixData->at(1), (quint32)ConversionTypeColor);

    // 4
    this->mOptions->setConvType(ConversionTypeMonochrome);
    QCOMPARE(this->mMatrixData->at(1), (quint32)ConversionTypeMonochrome);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::monoType()
{
    // 1
    this->mMatrixData->replace(2, MonochromeTypeEdge);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeEdge);

    // 2
    this->mMatrixData->replace(2, MonochromeTypeDiffuseDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeDiffuseDither);

    // 3
    this->mMatrixData->replace(2, MonochromeTypeOrderedDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeOrderedDither);

    // 4
    this->mMatrixData->replace(2, MonochromeTypeThresholdDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeThresholdDither);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setMonoType()
{
    // 1
    this->mOptions->setMonoType(MonochromeTypeEdge);
    QCOMPARE(this->mMatrixData->at(2), (quint32)MonochromeTypeEdge);

    // 2
    this->mOptions->setMonoType(MonochromeTypeDiffuseDither);
    QCOMPARE(this->mMatrixData->at(2), (quint32)MonochromeTypeDiffuseDither);

    // 3
    this->mOptions->setMonoType(MonochromeTypeOrderedDither);
    QCOMPARE(this->mMatrixData->at(2), (quint32)MonochromeTypeOrderedDither);

    // 4
    this->mOptions->setMonoType(MonochromeTypeThresholdDither);
    QCOMPARE(this->mMatrixData->at(2), (quint32)MonochromeTypeThresholdDither);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::edge()
{
    for (int i = -512; i < 512; i++)
    {
        this->mMatrixData->replace(3, (quint32)i);
        if (i >= 0 && i <= 255)
        {
            QCOMPARE(this->mOptions->edge(), i);
        }
        else
        {
            QVERIFY(this->mOptions->edge() >= 0);
            QVERIFY(this->mOptions->edge() <= 255);
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
            QCOMPARE((int)this->mMatrixData->at(3), i);
        }
        else
        {
            QVERIFY(this->mMatrixData->at(3) <= 255);
        }
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::blockSize()
{
    // 1
    this->mMatrixData->replace(4, Data8);
    QCOMPARE(this->mOptions->blockSize(), Data8);

    // 2
    this->mMatrixData->replace(4, Data16);
    QCOMPARE(this->mOptions->blockSize(), Data16);

    // 3
    this->mMatrixData->replace(4, Data24);
    QCOMPARE(this->mOptions->blockSize(), Data24);

    // 4
    this->mMatrixData->replace(4, Data32);
    QCOMPARE(this->mOptions->blockSize(), Data32);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setBlockSize()
{
    // 1
    this->mOptions->setBlockSize(Data8);
    QCOMPARE(this->mMatrixData->at(4), (quint32)Data8);

    // 2
    this->mOptions->setBlockSize(Data16);
    QCOMPARE(this->mMatrixData->at(4), (quint32)Data16);

    // 3
    this->mOptions->setBlockSize(Data24);
    QCOMPARE(this->mMatrixData->at(4), (quint32)Data24);

    // 4
    this->mOptions->setBlockSize(Data32);
    QCOMPARE(this->mMatrixData->at(4), (quint32)Data32);

    // 5
    this->mOptions->setBlockSize(Data8);
    QCOMPARE(this->mMatrixData->at(4), (quint32)Data8);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::rotate()
{
    // 1
    this->mMatrixData->replace(5, RotateNone);
    QCOMPARE(this->mOptions->rotate(), RotateNone);

    // 2
    this->mMatrixData->replace(5, Rotate90);
    QCOMPARE(this->mOptions->rotate(), Rotate90);

    // 3
    this->mMatrixData->replace(5, Rotate180);
    QCOMPARE(this->mOptions->rotate(), Rotate180);

    // 4
    this->mMatrixData->replace(5, Rotate270);
    QCOMPARE(this->mOptions->rotate(), Rotate270);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setRotate()
{
    // 1
    this->mOptions->setRotate(RotateNone);
    QCOMPARE(this->mMatrixData->at(5), (quint32)RotateNone);

    // 2
    this->mOptions->setRotate(Rotate90);
    QCOMPARE(this->mMatrixData->at(5), (quint32)Rotate90);

    // 3
    this->mOptions->setRotate(Rotate180);
    QCOMPARE(this->mMatrixData->at(5), (quint32)Rotate180);

    // 4
    this->mOptions->setRotate(Rotate270);
    QCOMPARE(this->mMatrixData->at(5), (quint32)Rotate270);

    // 5
    this->mOptions->setRotate(RotateNone);
    QCOMPARE(this->mMatrixData->at(5), (quint32)RotateNone);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::flipVertical()
{
    // 1
    this->mMatrixData->replace(6, 0);
    QCOMPARE(this->mOptions->flipVertical(), false);

    // 2
    this->mMatrixData->replace(6, 1);
    QCOMPARE(this->mOptions->flipVertical(), true);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setFlipVertical()
{
    // 1
    this->mOptions->setFlipVertical(false);
    QCOMPARE(this->mMatrixData->at(6), (quint32)0);

    // 2
    this->mOptions->setFlipVertical(true);
    QCOMPARE(this->mMatrixData->at(6), (quint32)1);

    // 3
    this->mOptions->setFlipVertical(false);
    QCOMPARE(this->mMatrixData->at(6), (quint32)0);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::flipHorizontal()
{
    // 1
    this->mMatrixData->replace(7, 0);
    QCOMPARE(this->mOptions->flipHorizontal(), false);

    // 2
    this->mMatrixData->replace(7, 1);
    QCOMPARE(this->mOptions->flipHorizontal(), true);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setFlipHorizontal()
{
    // 1
    this->mOptions->setFlipHorizontal(false);
    QCOMPARE(this->mMatrixData->at(7), (quint32)0);

    // 2
    this->mOptions->setFlipHorizontal(true);
    QCOMPARE(this->mMatrixData->at(7), (quint32)1);

    // 3
    this->mOptions->setFlipHorizontal(false);
    QCOMPARE(this->mMatrixData->at(7), (quint32)0);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::inverse()
{
    // 1
    this->mMatrixData->replace(8, 0);
    QCOMPARE(this->mOptions->inverse(), false);

    // 2
    this->mMatrixData->replace(8, 1);
    QCOMPARE(this->mOptions->inverse(), true);
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setInverse()
{
    // 1
    this->mOptions->setInverse(false);
    QCOMPARE(this->mMatrixData->at(8), (quint32)0);

    // 2
    this->mOptions->setInverse(true);
    QCOMPARE(this->mMatrixData->at(8), (quint32)1);

    // 3
    this->mOptions->setInverse(false);
    QCOMPARE(this->mMatrixData->at(8), (quint32)0);
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
        QCOMPARE(this->mOptions->maskUsed(), value1);

        this->mMatrixData->replace(10, value2);
        QCOMPARE(this->mOptions->maskAnd(), value2);

        this->mMatrixData->replace(11, value3);
        QCOMPARE(this->mOptions->maskOr(), value3);

        this->mMatrixData->replace(12, value4);
        QCOMPARE(this->mOptions->maskFill(), value4);

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
        QCOMPARE(this->mMatrixData->at(9), value1);

        this->mOptions->setMaskAnd(value2);
        QCOMPARE(this->mMatrixData->at(10), value2);

        this->mOptions->setMaskOr(value3);
        QCOMPARE(this->mMatrixData->at(11), value3);

        this->mOptions->setMaskFill(value4);
        QCOMPARE(this->mMatrixData->at(12), value4);

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
