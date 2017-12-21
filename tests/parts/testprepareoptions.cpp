#include "testprepareoptions.h"

#include "prepareoptions.h"

TestPrepareOptions::TestPrepareOptions(QObject *parent) :
  QObject(parent)
{
}

void TestPrepareOptions::initTestCase()
{
  this->mOptions = new Settings::Presets::PrepareOptions(this);
}

void TestPrepareOptions::convType()
{
  // 1
  this->mOptions->setConvType(Parsing::Conversion::Options::ConversionType::Monochrome);
  QCOMPARE(this->mOptions->convType(), Parsing::Conversion::Options::ConversionType::Monochrome);

  // 2
  this->mOptions->setConvType(Parsing::Conversion::Options::ConversionType::Grayscale);
  QCOMPARE(this->mOptions->convType(), Parsing::Conversion::Options::ConversionType::Grayscale);

  // 3
  this->mOptions->setConvType(Parsing::Conversion::Options::ConversionType::Color);
  QCOMPARE(this->mOptions->convType(), Parsing::Conversion::Options::ConversionType::Color);
}

void TestPrepareOptions::monoType()
{
  // 1
  this->mOptions->setMonoType(Parsing::Conversion::Options::MonochromeType::Edge);
  QCOMPARE(this->mOptions->monoType(), Parsing::Conversion::Options::MonochromeType::Edge);

  // 2
  this->mOptions->setMonoType(Parsing::Conversion::Options::MonochromeType::DiffuseDither);
  QCOMPARE(this->mOptions->monoType(), Parsing::Conversion::Options::MonochromeType::DiffuseDither);

  // 3
  this->mOptions->setMonoType(Parsing::Conversion::Options::MonochromeType::OrderedDither);
  QCOMPARE(this->mOptions->monoType(), Parsing::Conversion::Options::MonochromeType::OrderedDither);

  // 4
  this->mOptions->setMonoType(Parsing::Conversion::Options::MonochromeType::ThresholdDither);
  QCOMPARE(this->mOptions->monoType(), Parsing::Conversion::Options::MonochromeType::ThresholdDither);
}

void TestPrepareOptions::edge()
{
  for (int i = -512; i < 512; i++) {
    this->mOptions->setEdge(i);

    if (i >= 0 && i <= 255) {
      QCOMPARE(this->mOptions->edge(), i);
    } else {
      QVERIFY(this->mOptions->edge() >= 0);
      QVERIFY(this->mOptions->edge() <= 255);
    }
  }
}

void TestPrepareOptions::scanMain()
{
  // 1
  this->mOptions->setScanMain(Parsing::Conversion::Options::ScanMainDirection::TopToBottom);
  QCOMPARE(this->mOptions->scanMain(), Parsing::Conversion::Options::ScanMainDirection::TopToBottom);

  // 2
  this->mOptions->setScanMain(Parsing::Conversion::Options::ScanMainDirection::BottomToTop);
  QCOMPARE(this->mOptions->scanMain(), Parsing::Conversion::Options::ScanMainDirection::BottomToTop);

  // 3
  this->mOptions->setScanMain(Parsing::Conversion::Options::ScanMainDirection::LeftToRight);
  QCOMPARE(this->mOptions->scanMain(), Parsing::Conversion::Options::ScanMainDirection::LeftToRight);

  // 4
  this->mOptions->setScanMain(Parsing::Conversion::Options::ScanMainDirection::RightToLeft);
  QCOMPARE(this->mOptions->scanMain(), Parsing::Conversion::Options::ScanMainDirection::RightToLeft);
}

void TestPrepareOptions::scanSub()
{
  // 1
  this->mOptions->setScanSub(Parsing::Conversion::Options::ScanSubDirection::Forward);
  QCOMPARE(this->mOptions->scanSub(), Parsing::Conversion::Options::ScanSubDirection::Forward);

  // 2
  this->mOptions->setScanSub(Parsing::Conversion::Options::ScanSubDirection::Backward);
  QCOMPARE(this->mOptions->scanSub(), Parsing::Conversion::Options::ScanSubDirection::Backward);
}

void TestPrepareOptions::inverse()
{
  // 1
  this->mOptions->setInverse(false);
  QCOMPARE(this->mOptions->inverse(), false);

  // 2
  this->mOptions->setInverse(true);
  QCOMPARE(this->mOptions->inverse(), true);
}

void TestPrepareOptions::bandScanning()
{
  // 1
  this->mOptions->setBandScanning(false);
  QCOMPARE(this->mOptions->bandScanning(), false);

  // 2
  this->mOptions->setBandScanning(true);
  QCOMPARE(this->mOptions->bandScanning(), true);
}

void TestPrepareOptions::bandWidth()
{
  for (int i = -512; i < 512; i++) {
    this->mOptions->setBandWidth(i);

    if (i > 0) {
      QCOMPARE(this->mOptions->bandWidth(), i);
    } else {
      QCOMPARE(this->mOptions->bandWidth(), 1);
    }
  }
}

void TestPrepareOptions::cleanupTestCase()
{
  delete this->mOptions;
}

