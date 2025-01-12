#include "testconverterhelper.h"

#include <QVector>
#include <QCoreApplication>
#include <QRandomGenerator>
#include "qt-version-check.h"
#include "converterhelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"

TestConverterHelper::TestConverterHelper(QObject *parent) :
  QObject(parent)
{
}

void TestConverterHelper::initTestCase()
{
  this->mPreset = new Settings::Presets::Preset(this);
}

void TestConverterHelper::processPixels()
{
  const int count = 1000;
  this->mPreset->initColor(0, 5, 6, 5);
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x00000000);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // equals by default
  QVector<quint32> source, sample;
  QRandomGenerator prng(QTime::currentTime().msec());

  for (int i = 0; i < count; i++) {
    quint32 value = prng.generate();
    source << value;
    sample << value;
  }

  // no operations, no changes
  Parsing::Conversion::ConverterHelper::processPixels(this->mPreset, &sample);

  for (int i = 0; i < count; i++) {
    QCOMPARE(sample.at(i), source.at(i));
  }

  // add operations
  this->mPreset->matrix()->operationAdd(0x12345678, 3, true);
  this->mPreset->matrix()->operationAdd(0x87654321, 1, false);
  source.clear();
  sample.clear();

  for (int i = 0; i < count; i++) {
    quint32 value = prng.generate();
    quint32 a = 0;
    a |= (value & 0x12345678) << 3;
    a |= (value & 0x87654321) >> 1;
    source << a;
    sample << value;
  }

  Parsing::Conversion::ConverterHelper::processPixels(this->mPreset, &sample);

  for (int i = 0; i < count; i++) {
    QCOMPARE(sample.at(i), source.at(i));
  }

  // test AND mask
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->matrix()->setMaskAnd(0xabcdef01);
  this->mPreset->matrix()->setMaskOr(0x00000000);
  source.clear();
  sample.clear();

  for (int i = 0; i < count; i++) {
    quint32 value = prng.generate();
    quint32 a = value & 0xabcdef01;
    source << a;
    sample << value;
  }

  Parsing::Conversion::ConverterHelper::processPixels(this->mPreset, &sample);

  for (int i = 0; i < count; i++) {
    QCOMPARE(sample.at(i), source.at(i));
  }

  // test OR mask
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x1a5b5e4d);
  source.clear();
  sample.clear();

  for (int i = 0; i < count; i++) {
    quint32 value = prng.generate();
    quint32 a = value | 0x1a5b5e4d;
    source << a;
    sample << value;
  }

  Parsing::Conversion::ConverterHelper::processPixels(this->mPreset, &sample);

  for (int i = 0; i < count; i++) {
    QCOMPARE(sample.at(i), source.at(i));
  }
}

void TestConverterHelper::packData()
{
  this->mPreset->initColor(0, 5, 6, 5);
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x00000000);

  this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // test splitted data
  {
    this->mPreset->image()->setSplitToRows(true);

    QVector<quint32> source, expected;
    this->preparePackData(
      0x00ffffff, 0xffffffff,
      &source, 1000, 1000,
      true,
      &expected);

    QVector<quint32> sample;
    Parsing::Conversion::ConverterHelper::packData(this->mPreset,
        &source, 1000, 1000,
        &sample);

    QCOMPARE(sample.count(), expected.count());

    for (int i = 0; i < sample.count(); i++) {
      QCOMPARE(sample.at(i), expected.at(i));
    }
  }

  // test linear data
  {
    this->mPreset->image()->setSplitToRows(false);

    QVector<quint32> source, expected;
    this->preparePackData(
      0x00ffffff, 0xffffffff,
      &source, 1000, 1000,
      false,
      &expected);

    QVector<quint32> sample;
    Parsing::Conversion::ConverterHelper::packData(this->mPreset,
        &source, 1000, 1000,
        &sample);

    QCOMPARE(sample.count(), expected.count());

    for (int i = 0; i < sample.count(); i++) {
      QCOMPARE(sample.at(i), expected.at(i));
    }
  }
}

void TestConverterHelper::dataToString()
{
  const int count = 10;
  // fill source data
  QVector<quint32> source;
  QRandomGenerator prng(QTime::currentTime().msec());

  for (int y = 0; y < count; y++) {
    for (int x = 0; x < count; x++) {
      quint32 value = prng.generate();
      source << value;
    }
  }

  // configure matrix
  this->mPreset->initColor(0, 5, 6, 5);
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x00000000);
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // test splitted data
  {
    bool splitToRows = true;
    quint32 blocksPerLine = 0;
    this->mPreset->image()->setSplitToRows(splitToRows);
    this->mPreset->image()->setBlockPrefix("");
    this->mPreset->image()->setBlockSuffix("");
    this->mPreset->image()->setBlockDelimiter(", ");
    this->mPreset->image()->setBlocksPerLine(blocksPerLine);

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data8, &expected8);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data16, &expected16);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data24, &expected24);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data8);
    test8 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data16);
    test16 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data24);
    test24 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
    test32 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    // compare
    // this->compareStrings(test8, expected8);
    QCOMPARE(test8, expected8);
    // this->compareStrings(test16, expected16);
    QCOMPARE(test16, expected16);
    // this->compareStrings(test24, expected24);
    QCOMPARE(test24, expected24);
    // this->compareStrings(test32, expected32);
    QCOMPARE(test32, expected32);
  }

  // test data with specified number of blocks per line
  {
    bool splitToRows = true;
    quint32 blocksPerLine = 9;
    this->mPreset->image()->setSplitToRows(splitToRows);
    this->mPreset->image()->setBlockPrefix("");
    this->mPreset->image()->setBlockSuffix("");
    this->mPreset->image()->setBlockDelimiter(", ");
    this->mPreset->image()->setBlocksPerLine(blocksPerLine);

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data8, &expected8);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data16, &expected16);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data24, &expected24);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data8);
    test8 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data16);
    test16 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data24);
    test24 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
    test32 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    // compare
    QCOMPARE(test8, expected8);
    QCOMPARE(test16, expected16);
    QCOMPARE(test24, expected24);
    QCOMPARE(test32, expected32);
  }

  // test linear data
  {
    bool splitToRows = false;
    quint32 blocksPerLine = 0;
    this->mPreset->image()->setSplitToRows(splitToRows);
    this->mPreset->image()->setBlocksPerLine(blocksPerLine);

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data8, &expected8);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data16, &expected16);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data24, &expected24);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data8);
    test8 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data16);
    test16 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data24);
    test24 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
    test32 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    // compare
    QCOMPARE(test8, expected8);
    QCOMPARE(test16, expected16);
    QCOMPARE(test24, expected24);
    QCOMPARE(test32, expected32);
  }

  // test lineardata with specified number of blocks per line
  {
    bool splitToRows = false;
    quint32 blocksPerLine = 9;
    this->mPreset->image()->setSplitToRows(splitToRows);
    this->mPreset->image()->setBlocksPerLine(blocksPerLine);

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data8, &expected8);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data16, &expected16);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data24, &expected24);
    this->prepareStringData(&source, count, count, blocksPerLine, Settings::Presets::DataBlockSize::Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data8);
    test8 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data16);
    test16 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data24);
    test24 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
    test32 = Parsing::Conversion::ConverterHelper::dataToString(this->mPreset, &source);

    // compare
    QCOMPARE(test8, expected8);
    QCOMPARE(test16, expected16);
    QCOMPARE(test24, expected24);
    QCOMPARE(test32, expected32);
  }
}

void TestConverterHelper::uint2hex()
{
  quint32 value = 0x12345678;

  QBENCHMARK {
    Parsing::Conversion::ConverterHelper::uint2string(Settings::Presets::DataNumeralSystem::Hexadecimal, Settings::Presets::DataBlockSize::Data32, value);
  }
}

void TestConverterHelper::uint2octal()
{
  quint32 value = 0x12345678;

  QBENCHMARK {
    Parsing::Conversion::ConverterHelper::uint2string(Settings::Presets::DataNumeralSystem::Octal, Settings::Presets::DataBlockSize::Data32, value);
  }
}

void TestConverterHelper::uint2binary()
{
  quint32 value = 0x12345678;

  QBENCHMARK {
    Parsing::Conversion::ConverterHelper::uint2string(Settings::Presets::DataNumeralSystem::Binary, Settings::Presets::DataBlockSize::Data32, value);
  }
}

void TestConverterHelper::uint2string()
{
  QRandomGenerator prng(QTime::currentTime().msec());
  quint32 values[50];

  for (quint32 v = 0; v < sizeof(values) / sizeof(quint32); v++) {
    values[v] = prng.generate();
  }

  Settings::Presets::DataNumeralSystem nums[] = {
    Settings::Presets::DataNumeralSystem::Binary,
    Settings::Presets::DataNumeralSystem::Octal,
    Settings::Presets::DataNumeralSystem::Hexadecimal
  };

  Settings::Presets::DataBlockSize sizes[] = {
    Settings::Presets::DataBlockSize::Data8,
    Settings::Presets::DataBlockSize::Data16,
    Settings::Presets::DataBlockSize::Data24,
    Settings::Presets::DataBlockSize::Data32
  };

  int widths[3][4] = {
    { 8, 16, 24, 32 },
    { 3, 6, 8, 11 },
    { 2, 4, 6, 8 }
  };

  static const quint32 limits[4] = {
    0xfful,
    0xfffful,
    0xfffffful,
    0xfffffffful
  };

  for (quint32 v = 0; v < sizeof(values) / sizeof(quint32); v++) {
    for (quint32 n = 0; n < sizeof(nums) / sizeof(Settings::Presets::DataNumeralSystem); n++) {
      for (quint32 s = 0; s < sizeof(sizes) / sizeof(Settings::Presets::DataBlockSize); s++) {
        QString stringResult = Parsing::Conversion::ConverterHelper::uint2string(nums[n], sizes[s], values[v]);
        QString stringExpected = QString("%1").arg(values[v] & limits[static_cast<int>(sizes[s])], widths[n][s], static_cast<int>(nums[n]), QChar('0'));
        QCOMPARE(stringResult, stringExpected);
      }
    }
  }
}

void TestConverterHelper::jsengineSetProperty()
{
  int argc = 0;
  QCoreApplication app(argc, nullptr);

  {
    QImage image;
    TestConvImage cimage(&image, nullptr);
    cimage.setCondition(TestConvImage::CanBeDeleted);
  }

  {
    QImage image;
    TestConvImage cimage(&image, nullptr);
    cimage.setCondition(TestConvImage::CannotBeDeleted);
    QString err;
    Parsing::Conversion::ConverterHelper::collectPoints(&cimage, QString(), &err);
    cimage.setCondition(TestConvImage::CanBeDeleted);
  }
}

void TestConverterHelper::breakInfiniteScript()
{
  int argc = 0;
  QCoreApplication app(argc, nullptr);

  QString script = "for (var y = image.height - 1; y >= 0; y-=0) {\
                for (var x = image.width - 1; x >= 0; x--) {\
                    image.addPoint(x, y);\
                }\
            }";
  QImage image = QImage(20, 20, QImage::Format_ARGB32);
  TestConvImage cimage(&image, nullptr);
  cimage.setCondition(TestConvImage::CannotBeDeleted);
  QString err;
  Parsing::Conversion::ConverterHelper::collectPoints(&cimage, script, &err);
  cimage.setCondition(TestConvImage::CanBeDeleted);

  QVERIFY2(cimage.pointsCount() >= 100000, "Points count limit not reached. Value must be equals to 100k or 120% of width * height.");
}

void TestConverterHelper::cleanupTestCase()
{
  delete this->mPreset;
}

void TestConverterHelper::preparePackData(
  quint32 maskUsed, quint32 maskFill,
  QVector<quint32> *source, int width, int height,
  bool splitToRows,
  QVector<quint32> *packed)
{
  int packedRowWidth = 0;

  QRandomGenerator prng(QTime::currentTime().msec());

  // fill source data
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      quint32 value = prng.generate();
      value = 0x00ffff11;
      source->append(value);
    }
  }

  if (splitToRows) {
    // process each row
    for (int y = 0; y < height; y++) {
      QQueue<bool> bits;

      // stream row bits
      for (int x = 0; x < width; x++) {
        quint32 value = source->at(x + width * y);

        for (int j = 0; j < 32; j++) {
          if ((maskUsed & (0x80000000 >> j)) != 0) {
            bits.enqueue((value & (0x80000000 >> j)) != 0);
          }
        }
      }

      // pack bits
      int counter = 0;

      while (!bits.empty()) {
        quint32 value = 0;

        for (int j = 0; j < 32 && !bits.empty(); j++) {
          if ((maskFill & (0x80000000 >> j)) != 0) {
            bool bit = bits.dequeue();

            if (bit) {
              value |= (0x80000000 >> j);
            }
          }
        }

        packed->append(value);
        counter++;
      }

      packedRowWidth = qMax(packedRowWidth, counter);
    }
  } else {
    // process entire data
    QQueue<bool> bits;

    // stream row bits
    for (int i = 0; i < source->size(); i++) {
      quint32 value = source->at(i);

      for (int j = 0; j < 32; j++) {
        if ((maskUsed & (0x80000000 >> j)) != 0) {
          bits.enqueue((value & (0x80000000 >> j)) != 0);
        }
      }
    }

    // pack bits
    int counter = 0;

    while (!bits.empty()) {
      quint32 value = 0;

      for (int j = 0; j < 32 && !bits.empty(); j++) {
        if ((maskFill & (0x80000000 >> j)) != 0) {
          bool bit = bits.dequeue();

          if (bit) {
            value |= (0x80000000 >> j);
          }
        }
      }

      packed->append(value);
      counter++;
    }

    packedRowWidth = counter;
    height = 1;
  }
}

void TestConverterHelper::prepareStringData(
  QVector<quint32> *source, int width, int height,
  int blocksPerLine,
  Parsing::Conversion::Options::DataBlockSize size, QString *string)
{
  QString result;
  quint32 mask = 0;
  int digits = 1;

  if (blocksPerLine < 1) {
    blocksPerLine = 0;
  }

  switch (size) {
    case Parsing::Conversion::Options::DataBlockSize::Data8: {
      mask = 0x000000ff;
      digits = 2;
      break;
    }

    case Parsing::Conversion::Options::DataBlockSize::Data16: {
      mask = 0x0000ffff;
      digits = 4;
      break;
    }

    case Parsing::Conversion::Options::DataBlockSize::Data24: {
      mask = 0x00ffffff;
      digits = 6;
      break;
    }

    case Parsing::Conversion::Options::DataBlockSize::Data32: {
      mask = 0xffffffff;
      digits = 8;
      break;
    }
  }

  QStringList list;

  for (auto i = 0; i < source->count(); i++) {
    quint32 value = source->at(i);
    list.append(QString("%1, ").arg(value & mask, digits, 16, QChar('0')));
  }

  if (blocksPerLine < 1) {
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        result.append(list.at(x + width * y));
      }
    }
  } else {
    int blockerPerLineCounter = 0;

    for (int i = 0; i < source->count(); i++) {
      result.append(list.at(i));
      blockerPerLineCounter++;

      if ((blocksPerLine > 0) && (blockerPerLineCounter >= blocksPerLine)) {
        blockerPerLineCounter = 0;
        result.append("\n");
      }
    }
  }

  result = result.trimmed();

  if (result.endsWith(",")) {
    result.truncate(result.length() - 1);
  }

  *string = result;
}

void TestConverterHelper::compareStrings(const QString &actual, const QString &expected)
{
  if (actual != expected) {
    qDebug() << "Strings do not match:";
    qDebug() << "Actual:   " << actual;
    qDebug() << "Expected: " << expected;

    int minLength = qMin(actual.length(), expected.length());

    for (int i = 0; i < minLength; ++i) {
      if (actual[i] != expected[i]) {
        qDebug() << "Difference at position" << i << ": Actual='" << actual[i] << "', Expected='" << expected[i] << "'";
      }
    }

    if (actual.length() > expected.length()) {
      qDebug() << "Actual string is longer by" << actual.length() - expected.length() << "characters.";
    } else if (expected.length() > actual.length()) {
      qDebug() << "Expected string is longer by" << expected.length() - actual.length() << "characters.";
    }
  }
}

TestConvImage::TestConvImage(const QImage *image, QObject *parent) :
  Parsing::Conversion::ConvImageScan(image, parent)
{
  this->mCondition = CanBeDeleted;
}

TestConvImage::~TestConvImage()
{
  if (this->mCondition != CanBeDeleted) {
    QFAIL("The object, passed to the script engine as property, was unexpectedly destroyed.");
  }
}

void TestConvImage::setCondition(TestConvImage::Condition value)
{
  this->mCondition = value;
}

