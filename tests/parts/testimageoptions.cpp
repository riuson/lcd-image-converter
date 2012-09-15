#include "testimageoptions.h"
//-----------------------------------------------------------------------------
#include "imageoptions.h"
//-----------------------------------------------------------------------------
TestImageOptions::TestImageOptions(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestImageOptions::initTestCase()
{
    this->mOptions = new ImageOptions(this);
}
//-----------------------------------------------------------------------------
void TestImageOptions::bytesOrder()
{
    // 1
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderBigEndian);

    // 2
    this->mOptions->setBytesOrder(BytesOrderLittleEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderLittleEndian);
}
//-----------------------------------------------------------------------------
void TestImageOptions::blockSize()
{
    // 1
    this->mOptions->setBlockSize(Data8);
    QCOMPARE(this->mOptions->blockSize(), Data8);

    // 2
    this->mOptions->setBlockSize(Data16);
    QCOMPARE(this->mOptions->blockSize(), Data16);

    // 3
    this->mOptions->setBlockSize(Data24);
    QCOMPARE(this->mOptions->blockSize(), Data24);

    // 4
    this->mOptions->setBlockSize(Data32);
    QCOMPARE(this->mOptions->blockSize(), Data32);
}
//-----------------------------------------------------------------------------
void TestImageOptions::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
