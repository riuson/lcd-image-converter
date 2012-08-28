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
void TestConversionMatrixOptions::cleanupTestCase()
{
    delete this->mOptions;
    delete this->mMatrixData;
}
//-----------------------------------------------------------------------------
