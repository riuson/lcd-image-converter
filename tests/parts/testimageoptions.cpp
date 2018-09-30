#include "testimageoptions.h"

#include "imageoptions.h"
#include "conversion_options.h"

TestImageOptions::TestImageOptions(QObject *parent) :
  QObject(parent)
{
}

void TestImageOptions::initTestCase()
{
  this->mOptions = new Settings::Presets::ImageOptions(this);
}

void TestImageOptions::bytesOrder()
{
  // 1
  this->mOptions->setBytesOrder(Parsing::Conversion::Options::BytesOrder::BigEndian);
  QCOMPARE(this->mOptions->bytesOrder(), Parsing::Conversion::Options::BytesOrder::BigEndian);

  // 2
  this->mOptions->setBytesOrder(Parsing::Conversion::Options::BytesOrder::LittleEndian);
  QCOMPARE(this->mOptions->bytesOrder(), Parsing::Conversion::Options::BytesOrder::LittleEndian);
}

void TestImageOptions::blockSize()
{
  // 1
  this->mOptions->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data8);
  QCOMPARE(this->mOptions->blockSize(), Parsing::Conversion::Options::DataBlockSize::Data8);

  // 2
  this->mOptions->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data16);
  QCOMPARE(this->mOptions->blockSize(), Parsing::Conversion::Options::DataBlockSize::Data16);

  // 3
  this->mOptions->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data24);
  QCOMPARE(this->mOptions->blockSize(), Parsing::Conversion::Options::DataBlockSize::Data24);

  // 4
  this->mOptions->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data32);
  QCOMPARE(this->mOptions->blockSize(), Parsing::Conversion::Options::DataBlockSize::Data32);
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

