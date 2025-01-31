#include "testrle.h"

#include <QVector>

#include "rlecompressor.h"

TestRle::TestRle(QObject* parent) : QObject(parent) {}

void TestRle::initTestCase() {}

void TestRle::compress1()
{
  QVector<quint32> testVector1, testVector2;

  testVector1 << 1 << 2 << 2 << 2 << 3 << 3 << 4 << 5 << 5 << 5 << 5 << 5 << 6 << 7 << 8 << 8 << 8 << 8 << 9 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10;

  testVector2 << 1 << 1 << 3 << 2 << 2 << 3 << 1 << 4 << 5 << 5 << 1 << 6 << 1 << 7 << 4 << 8 << 1 << 9 << 127 << 10
              << 3 << 10;

  Utils::Compression::RleCompressor compressor;

  QVector<quint32> test;
  compressor.compress(&testVector1, Parsing::Conversion::Options::DataBlockSize::Data8, &test, 1);

  QCOMPARE(test.size(), testVector2.size());

  for (int i = 0; i < test.size(); i++) {
    QCOMPARE(test.at(i), testVector2.at(i));
  }
}

void TestRle::compress2()
{
  QVector<quint32> testVector1, testVector2;

  testVector1 << 1 << 2 << 2 << 2 << 3 << 3 << 4 << 5 << 5 << 5 << 5 << 5 << 6 << 7 << 8 << 8 << 8 << 8 << 9 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10;

  testVector2 << 1 << 1 << 3 << 2 << 2 << 3 << 1 << 4 << ((quint32)5) << 5 << ((quint32)-2) << 6 << 7 << 4 << 8 << 1
              << 9 << 127 << 10 << 3 << 10;

  Utils::Compression::RleCompressor compressor;

  QVector<quint32> test;
  compressor.compress(&testVector1, Parsing::Conversion::Options::DataBlockSize::Data8, &test, 2);

  QCOMPARE(test.size(), testVector2.size());

  for (int i = 0; i < test.size(); i++) {
    QCOMPARE(test.at(i), testVector2.at(i));
  }
}

void TestRle::compress5()
{
  QVector<quint32> testVector1, testVector2;

  testVector1 << 1 << 2 << 2 << 2 << 3 << 3 << 4 << 5 << 5 << 5 << 5 << 5 << 6 << 7 << 8 << 8 << 8 << 8 << 9 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10
              << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10;

  testVector2 << ((quint32)-7) << 1 << 2 << 2 << 2 << 3 << 3 << 4 << ((quint32)5) << 5 << ((quint32)-7) << 6 << 7 << 8
              << 8 << 8 << 8 << 9 << 127 << 10 << 3 << 10;

  Utils::Compression::RleCompressor compressor;

  QVector<quint32> test;
  compressor.compress(&testVector1, Parsing::Conversion::Options::DataBlockSize::Data8, &test, 5);

  QCOMPARE(test.size(), testVector2.size());

  for (int i = 0; i < test.size(); i++) {
    QCOMPARE(test.at(i), testVector2.at(i));
  }
}

void TestRle::cleanupTestCase() {}
