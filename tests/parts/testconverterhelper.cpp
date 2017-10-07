#include "testconverterhelper.h"

#include <QVector>
#include <QCoreApplication>
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
  this->mPreset = new Preset(this);
}

void TestConverterHelper::processPixels()
{
  const int count = 1000;
  this->mPreset->initColor(0, 5, 6, 5);
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->image()->setBlockSize(Data32);
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x00000000);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // equals by default
  QVector<quint32> source, sample;
  qsrand(QTime::currentTime().msec());

  for (int i = 0; i < count; i++) {
    quint32 value = qrand();
    source << value;
    sample << value;
  }

  // no operations, no changes
  ConverterHelper::processPixels(this->mPreset, &sample);

  for (int i = 0; i < count; i++) {
    QCOMPARE(sample.at(i), source.at(i));
  }

  // add operations
  this->mPreset->matrix()->operationAdd(0x12345678, 3, true);
  this->mPreset->matrix()->operationAdd(0x87654321, 1, false);
  source.clear();
  sample.clear();

  for (int i = 0; i < count; i++) {
    quint32 value = qrand();
    quint32 a = 0;
    a |= (value & 0x12345678) << 3;
    a |= (value & 0x87654321) >> 1;
    source << a;
    sample << value;
  }

  ConverterHelper::processPixels(this->mPreset, &sample);

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
    quint32 value = qrand();
    quint32 a = value & 0xabcdef01;
    source << a;
    sample << value;
  }

  ConverterHelper::processPixels(this->mPreset, &sample);

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
    quint32 value = qrand();
    quint32 a = value | 0x1a5b5e4d;
    source << a;
    sample << value;
  }

  ConverterHelper::processPixels(this->mPreset, &sample);

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

  this->mPreset->image()->setBlockSize(Data32);
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // test splitted data
  {
    this->mPreset->image()->setSplitToRows(true);

    QVector<quint32> source, expected;
    int widthExpected, heightExpected;
    this->preparePackData(
      0x00ffffff, 0xffffffff,
      &source, 1000, 1000,
      true,
      &expected, &widthExpected, &heightExpected);

    QVector<quint32> sample;
    int widthSample, heightSample;
    ConverterHelper::packData(this->mPreset,
                              &source, 1000, 1000,
                              &sample, &widthSample, &heightSample);

    QCOMPARE(widthSample, widthExpected);
    QCOMPARE(heightSample, heightExpected);
    QCOMPARE(sample.count(), expected.count());

    for (int i = 0; i < sample.count(); i++) {
      QCOMPARE(sample.at(i), expected.at(i));
    }
  }

  // test linear data
  {
    this->mPreset->image()->setSplitToRows(false);

    QVector<quint32> source, expected;
    int widthExpected, heightExpected;
    this->preparePackData(
      0x00ffffff, 0xffffffff,
      &source, 1000, 1000,
      false,
      &expected, &widthExpected, &heightExpected);

    QVector<quint32> sample;
    int widthSample, heightSample;
    ConverterHelper::packData(this->mPreset,
                              &source, 1000, 1000,
                              &sample, &widthSample, &heightSample);

    QCOMPARE(widthSample, widthExpected);
    QCOMPARE(heightSample, heightExpected);
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
  qsrand(QTime::currentTime().msec());

  for (int y = 0; y < count; y++) {
    for (int x = 0; x < count; x++) {
      quint32 value = qrand();
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
    this->mPreset->image()->setSplitToRows(true);
    this->mPreset->image()->setBlockPrefix("");
    this->mPreset->image()->setBlockSuffix("");
    this->mPreset->image()->setBlockDelimiter(", ");

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, true, Data8, &expected8);
    this->prepareStringData(&source, count, count, true, Data16, &expected16);
    this->prepareStringData(&source, count, count, true, Data24, &expected24);
    this->prepareStringData(&source, count, count, true, Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Data8);
    test8 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data16);
    test16 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data24);
    test24 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data32);
    test32 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    // compare
    QCOMPARE(test8, expected8);
    QCOMPARE(test16, expected16);
    QCOMPARE(test24, expected24);
    QCOMPARE(test32, expected32);
  }
  // test linear data
  {
    this->mPreset->image()->setSplitToRows(false);

    // create expected strings
    QString expected8, expected16, expected24, expected32;

    this->prepareStringData(&source, count, count, false, Data8, &expected8);
    this->prepareStringData(&source, count, count, false, Data16, &expected16);
    this->prepareStringData(&source, count, count, false, Data24, &expected24);
    this->prepareStringData(&source, count, count, false, Data32, &expected32);

    // create test strings
    QString test8, test16, test24, test32;

    this->mPreset->image()->setBlockSize(Data8);
    test8 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data16);
    test16 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data24);
    test24 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    this->mPreset->image()->setBlockSize(Data32);
    test32 = ConverterHelper::dataToString(this->mPreset, &source, count, count);

    // compare
    QCOMPARE(test8, expected8);
    QCOMPARE(test16, expected16);
    QCOMPARE(test24, expected24);
    QCOMPARE(test32, expected32);
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
    ConverterHelper::collectPoints(&cimage, QString(), &err);
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
  ConverterHelper::collectPoints(&cimage, script, &err);
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
  QVector<quint32> *packed, int *widthOut, int *heightOut)
{
  int packedRowWidth = 0;

  qsrand(QTime::currentTime().msec());

  // fill source data
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      quint32 value = qrand();
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

  *widthOut = packedRowWidth;
  *heightOut = height;
}

void TestConverterHelper::prepareStringData(
  QVector<quint32> *source, int width, int height,
  bool splitToRows,
  DataBlockSize size, QString *string)
{
  QString result;
  quint32 mask = 0;
  int digits = 1;

  switch (size) {
    case Data8: {
      mask = 0x000000ff;
      digits = 2;
      break;
    }

    case Data16: {
      mask = 0x0000ffff;
      digits = 4;
      break;
    }

    case Data24: {
      mask = 0x00ffffff;
      digits = 6;
      break;
    }

    case Data32: {
      mask = 0xffffffff;
      digits = 8;
      break;
    }
  }

  if (splitToRows) {
    for (int y = 0; y < height; y++) {
      if (y > 0) {
        result.append("\n");
      }

      for (int x = 0; x < width; x++) {
        quint32 value = source->at(x + width * y);
        result.append(QString("%1, ").arg(value & mask, digits, 16, QChar('0')));
      }
    }
  } else {
    for (int i = 0; i < width; i++) {
      quint32 value = source->at(i);
      result.append(QString("%1, ").arg(value & mask, digits, 16, QChar('0')));
    }
  }

  result.truncate(result.length() - 2);
  *string = result;
}

TestConvImage::TestConvImage(const QImage *image, QObject *parent) :
  ConvImageScan(image, parent)
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

