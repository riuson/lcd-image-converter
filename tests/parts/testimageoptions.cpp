#include "testimageoptions.h"

#include "imageoptions.h"

TestImageOptions::TestImageOptions(QObject *parent) :
    QObject(parent)
{
}

void TestImageOptions::initTestCase()
{
    this->mOptions = new ImageOptions(this);
}

void TestImageOptions::bytesOrder()
{
    // 1
    this->mOptions->setBytesOrder(BytesOrderBigEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderBigEndian);

    // 2
    this->mOptions->setBytesOrder(BytesOrderLittleEndian);
    QCOMPARE(this->mOptions->bytesOrder(), BytesOrderLittleEndian);
}

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

void TestImageOptions::blockDefaultOnes()
{
    // 1
    this->mOptions->setBlockDefaultOnes(false);
    QCOMPARE(this->mOptions->blockDefaultOnes(), false);

    // 2
    this->mOptions->setBlockDefaultOnes(true);
    QCOMPARE(this->mOptions->blockDefaultOnes(), true);

    // 3
    this->mOptions->setBlockDefaultOnes(false);
    QCOMPARE(this->mOptions->blockDefaultOnes(), false);
}

void TestImageOptions::splitToRows()
{
    // 1
    this->mOptions->setSplitToRows(false);
    QCOMPARE(this->mOptions->splitToRows(), false);

    // 2
    this->mOptions->setSplitToRows(true);
    QCOMPARE(this->mOptions->splitToRows(), true);
}

void TestImageOptions::compressionRle()
{
    // 1
    this->mOptions->setCompressionRle(false);
    QCOMPARE(this->mOptions->compressionRle(), false);

    // 2
    this->mOptions->setCompressionRle(true);
    QCOMPARE(this->mOptions->compressionRle(), true);
}

void TestImageOptions::blockPrefix()
{
    // 1
    this->mOptions->setBlockPrefix("prefix1");
    QCOMPARE(this->mOptions->blockPrefix(), QString("prefix1"));

    // 2
    this->mOptions->setBlockPrefix("prefix2");
    QCOMPARE(this->mOptions->blockPrefix(), QString("prefix2"));

    // 3
    this->mOptions->setBlockPrefix("prefix3");
    QCOMPARE(this->mOptions->blockPrefix(), QString("prefix3"));
}

void TestImageOptions::blockSuffix()
{
    // 1
    this->mOptions->setBlockSuffix("suffix1");
    QCOMPARE(this->mOptions->blockSuffix(), QString("suffix1"));

    // 2
    this->mOptions->setBlockSuffix("suffix2");
    QCOMPARE(this->mOptions->blockSuffix(), QString("suffix2"));

    // 3
    this->mOptions->setBlockSuffix("suffix3");
    QCOMPARE(this->mOptions->blockSuffix(), QString("suffix3"));
}

void TestImageOptions::blockDelimiter()
{
    // 1
    this->mOptions->setBlockDelimiter("delimiter3");
    QCOMPARE(this->mOptions->blockDelimiter(), QString("delimiter3"));

    // 2
    this->mOptions->setBlockDelimiter("delimiter3");
    QCOMPARE(this->mOptions->blockDelimiter(), QString("delimiter3"));

    // 3
    this->mOptions->setBlockDelimiter("delimiter3");
    QCOMPARE(this->mOptions->blockDelimiter(), QString("delimiter3"));
}

void TestImageOptions::cleanupTestCase()
{
    delete this->mOptions;
}

