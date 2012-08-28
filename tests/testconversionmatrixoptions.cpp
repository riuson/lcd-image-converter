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
    if (this->mMatrixData->count() != 5)
        QFAIL("Matrix options not initialized");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::bytesOrder()
{
    quint32 value = this->mMatrixData->at(0);
    const quint32 mask = 0x40000000;

    // 1
    value |= mask;
    this->mMatrixData->replace(0, value);
    if (this->mOptions->bytesOrder() != BytesOrderBigEndian)
        QFAIL("Big-Endian failed");

    // 2
    value &= ~mask;
    this->mMatrixData->replace(0, value);
    if (this->mOptions->bytesOrder() != BytesOrderLittleEndian)
        QFAIL("Little-Endian failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setBytesOrder()
{
    const quint32 mask = 0x40000000;

    // 1
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    if ((this->mMatrixData->at(0) & mask) == 0)
        QFAIL("Big-Endian failed");

    // 2
    this->mOptions->setBytesOrder(BytesOrderLittleEndian);
    if ((this->mMatrixData->at(0) & mask) != 0)
        QFAIL("Little-Endian failed");

    // 3
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    if ((this->mMatrixData->at(0) & mask) == 0)
        QFAIL("Big-Endian failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::convType()
{
    quint32 value = this->mMatrixData->at(0);
    const quint32 mask = 0x30000000;

    // 1
    value &= ~mask;
    this->mMatrixData->replace(0, value);
    if (this->mOptions->convType() != ConversionTypeMonochrome)
        QFAIL("Monochrome failed");

    // 2
    value &= ~mask;
    value |= 0x10000000;
    this->mMatrixData->replace(0, value);
    if (this->mOptions->convType() != ConversionTypeGrayscale)
        QFAIL("Grayscale failed");

    // 3
    value &= ~mask;
    value |= 0x20000000;
    this->mMatrixData->replace(0, value);
    if (this->mOptions->convType() != ConversionTypeColor)
        QFAIL("Color failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::setConvType()
{
    const quint32 mask = 0x30000000;

    // 1
    this->mOptions->setConvType(ConversionTypeMonochrome);
    if ((this->mMatrixData->at(0) & mask) != 0)
        QFAIL("Monochrome failed");

    // 2
    this->mOptions->setConvType(ConversionTypeGrayscale);
    if ((this->mMatrixData->at(0) & mask) != 0x10000000)
        QFAIL("Grayscale failed");

    // 3
    this->mOptions->setConvType(ConversionTypeColor);
    if ((this->mMatrixData->at(0) & mask) != 0x20000000)
        QFAIL("Color failed");

    // 4
    this->mOptions->setConvType(ConversionTypeMonochrome);
    if ((this->mMatrixData->at(0) & mask) != 0)
        QFAIL("Monochrome failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrixOptions::cleanupTestCase()
{
    delete this->mOptions;
    delete this->mMatrixData;
}
//-----------------------------------------------------------------------------
