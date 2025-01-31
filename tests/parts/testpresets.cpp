#include "testpresets.h"

#include "imageoptions.h"
#include "matrixoptions.h"
#include "prepareoptions.h"
#include "preset.h"

TestPresets::TestPresets(QObject* parent) : QObject(parent) {}

void TestPresets::initTestCase() { this->mOptions = new Settings::Presets::Preset(this); }

void TestPresets::loadSave()
{
  Settings::Presets::Preset source;
  source.initColor(0, 5, 6, 5);
  source.prepare()->setScanMain(Parsing::Conversion::Options::ScanMainDirection::BottomToTop);
  source.prepare()->setScanSub(Parsing::Conversion::Options::ScanSubDirection::Backward);
  source.prepare()->setBandScanning(true);
  source.prepare()->setBandWidth(577);
  source.image()->setBytesOrder(Parsing::Conversion::Options::BytesOrder::BigEndian);
  source.image()->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data16);
  source.image()->setBlockDefaultOnes(true);
  source.image()->setSplitToRows(true);
  source.image()->setCompressionRle(true);
  source.image()->setBlockPrefix("prefix");
  source.image()->setBlockSuffix("suffix");
  source.image()->setBlockDelimiter("delimiter");

  this->mOptions->initColor(0, 5, 6, 5);
  this->mOptions->prepare()->setScanMain(Parsing::Conversion::Options::ScanMainDirection::BottomToTop);
  this->mOptions->prepare()->setScanSub(Parsing::Conversion::Options::ScanSubDirection::Backward);
  this->mOptions->prepare()->setBandScanning(true);
  this->mOptions->prepare()->setBandWidth(577);
  this->mOptions->matrix()->operationsRemoveAll();
  this->mOptions->image()->setBytesOrder(Parsing::Conversion::Options::BytesOrder::BigEndian);
  this->mOptions->image()->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data16);
  this->mOptions->image()->setBlockDefaultOnes(true);
  this->mOptions->image()->setSplitToRows(true);
  this->mOptions->image()->setCompressionRle(true);
  this->mOptions->image()->setBlockPrefix("prefix");
  this->mOptions->image()->setBlockSuffix("suffix");
  this->mOptions->image()->setBlockDelimiter("delimiter");

  for (int i = 0; i < 10; i++) {
    this->mOptions->matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
    source.matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
  }

  const QString name = "testpresets";
  this->mOptions->save(name);

  this->mOptions->matrix()->operationsRemoveAll();
  this->mOptions->initMono(Parsing::Conversion::Options::MonochromeType::Edge, 1);

  if (!this->mOptions->load(name)) {
    QFAIL("preset not loaded");
    // test failed under Windows 7, possible because of this:
    // https://bugreports.qt-project.org/browse/QTBUG-24145
    // https://codereview.qt-project.org/#change,17656
  }

  QCOMPARE(this->mOptions->prepare()->convType(), source.prepare()->convType());
  QCOMPARE(this->mOptions->prepare()->monoType(), source.prepare()->monoType());
  QCOMPARE(this->mOptions->prepare()->edge(), source.prepare()->edge());
  QCOMPARE(this->mOptions->prepare()->scanMain(), source.prepare()->scanMain());
  QCOMPARE(this->mOptions->prepare()->scanSub(), source.prepare()->scanSub());
  QCOMPARE(this->mOptions->prepare()->inverse(), source.prepare()->inverse());
  QCOMPARE(this->mOptions->prepare()->bandScanning(), source.prepare()->bandScanning());
  QCOMPARE(this->mOptions->prepare()->bandWidth(), source.prepare()->bandWidth());

  QCOMPARE(this->mOptions->matrix()->maskUsed(), source.matrix()->maskUsed());
  QCOMPARE(this->mOptions->matrix()->maskAnd(), source.matrix()->maskAnd());
  QCOMPARE(this->mOptions->matrix()->maskOr(), source.matrix()->maskOr());
  QCOMPARE(this->mOptions->matrix()->maskFill(), source.matrix()->maskFill());

  QCOMPARE(this->mOptions->image()->bytesOrder(), source.image()->bytesOrder());
  QCOMPARE(this->mOptions->image()->blockSize(), source.image()->blockSize());
  QCOMPARE(this->mOptions->image()->blockDefaultOnes(), source.image()->blockDefaultOnes());
  QCOMPARE(this->mOptions->image()->splitToRows(), source.image()->splitToRows());
  QCOMPARE(this->mOptions->image()->compressionRle(), source.image()->compressionRle());
  QCOMPARE(this->mOptions->image()->blockPrefix(), source.image()->blockPrefix());
  QCOMPARE(this->mOptions->image()->blockSuffix(), source.image()->blockSuffix());
  QCOMPARE(this->mOptions->image()->blockDelimiter(), source.image()->blockDelimiter());
}

void TestPresets::cleanupTestCase() { delete this->mOptions; }
