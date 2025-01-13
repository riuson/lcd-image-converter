#include "testmatrixoptions.h"

#include "matrixoptions.h"

TestMatrixOptions::TestMatrixOptions(QObject* parent) : QObject(parent) {}

void TestMatrixOptions::initTestCase() { this->mOptions = new Settings::Presets::MatrixOptions(this); }

void TestMatrixOptions::mask()
{
  quint32 value1 = 0x12345678;
  quint32 value2 = 0x23456789;
  quint32 value3 = 0x57890155;
  quint32 value4 = 0x4687489a;

  for (int i = 0; i < 4; i++) {
    this->mOptions->setMaskUsed(value1);
    QCOMPARE(this->mOptions->maskUsed(), value1);

    this->mOptions->setMaskAnd(value2);
    QCOMPARE(this->mOptions->maskAnd(), value2);

    this->mOptions->setMaskOr(value3);
    QCOMPARE(this->mOptions->maskOr(), value3);

    this->mOptions->setMaskFill(value4);
    QCOMPARE(this->mOptions->maskFill(), value4);

    value1 = value1 >> 1;
    value2 = value2 >> 2;
    value3 = value3 >> 3;
    value4 = value4 >> 4;
  }
}

void TestMatrixOptions::operationsCount()
{
  this->mOptions->operationsRemoveAll();

  this->mOptions->operationAdd(0x12345678, 0, false);
  QCOMPARE(this->mOptions->operationsCount(), 1);

  this->mOptions->operationAdd(0x23131315, 1, true);
  QCOMPARE(this->mOptions->operationsCount(), 2);
}

void TestMatrixOptions::operation()
{
  this->mOptions->operationsRemoveAll();

  quint32 expectedMask = 0x12345678;
  int expectedShift = 10;
  bool expectedLeft = true;

  for (int i = 0; i < 20; i++) {
    this->mOptions->operationAdd(expectedMask, expectedShift, expectedLeft);

    quint32 actualMask;
    int actualShift;
    bool actualLeft;
    this->mOptions->operation(i, &actualMask, &actualShift, &actualLeft);

    if (expectedShift >= 0) {
      QCOMPARE(actualMask, expectedMask);
      QCOMPARE(actualShift, expectedShift);
      QCOMPARE(actualLeft, expectedLeft);
    } else {
      QCOMPARE(actualMask, expectedMask);
      QVERIFY(actualShift >= 0);
      QCOMPARE(actualLeft, expectedLeft);
    }

    expectedMask = expectedMask >> 3;
    expectedShift--;
    expectedLeft = !expectedLeft;
  }
}

void TestMatrixOptions::operationAdd()
{
  this->mOptions->operationsRemoveAll();

  quint32 expectedMask = 0x41654649;
  int expectedShift = 10;
  bool expectedLeft = true;

  for (int i = 0; i < 20; i++) {
    this->mOptions->operationAdd(expectedMask, expectedShift, expectedLeft);

    quint32 actualMask;
    int actualShift;
    bool actualLeft;
    this->mOptions->operation(i, &actualMask, &actualShift, &actualLeft);

    if (expectedShift >= 0) {
      QCOMPARE(actualMask, expectedMask);
      QCOMPARE(actualShift, expectedShift);
      QCOMPARE(actualLeft, expectedLeft);
    } else {
      QCOMPARE(actualMask, expectedMask);
      QVERIFY(actualShift >= 0);
      QCOMPARE(actualLeft, expectedLeft);
    }

    expectedMask = expectedMask << 1;
    expectedShift--;
    expectedLeft = !expectedLeft;
  }
}

void TestMatrixOptions::operationRemove()
{
  this->mOptions->operationsRemoveAll();

  this->mOptions->operationAdd(0x12345678, 5, false);
  this->mOptions->operationAdd(0x87654321, 3, true);

  quint32 actualMask;
  int actualShift;
  bool actualLeft;

  this->mOptions->operationRemove(0);

  this->mOptions->operation(0, &actualMask, &actualShift, &actualLeft);

  if (this->mOptions->operationsCount() != 1) {
    QFAIL("operationRemove failed");
  }

  QCOMPARE(actualMask, 0x87654321);
  QCOMPARE(actualShift, 3);
  QCOMPARE(actualLeft, true);
}

void TestMatrixOptions::operationRemoveAll()
{
  this->mOptions->operationsRemoveAll();

  quint32 mask = 0x41654649;
  int shift = 10;
  bool left = true;

  for (int i = 0; i < 20; i++) {
    this->mOptions->operationAdd(mask, shift, left);
  }

  QCOMPARE(this->mOptions->operationsCount(), 20);

  this->mOptions->operationsRemoveAll();
  QCOMPARE(this->mOptions->operationsCount(), 0);
}

void TestMatrixOptions::operationReplace()
{
  this->mOptions->operationsRemoveAll();

  this->mOptions->operationAdd(0x12345678, 5, false);
  this->mOptions->operationAdd(0x87654321, 3, true);
  this->mOptions->operationAdd(0x98abcdef, 7, false);
  this->mOptions->operationAdd(0xdf546b01, 2, true);

  QCOMPARE(this->mOptions->operationsCount(), 4);

  quint32 mask;
  int shift;
  bool left;

  // 1
  this->mOptions->operationReplace(2, 0x1234f00d, 0, true);

  this->mOptions->operation(2, &mask, &shift, &left);

  QCOMPARE(this->mOptions->operationsCount(), 4);

  QCOMPARE(mask, (quint32)0x1234f00d);
  QCOMPARE(shift, 0);
  QCOMPARE(left, true);

  // 2
  this->mOptions->operationReplace(2, 0xab154e46, 7, false);

  this->mOptions->operation(2, &mask, &shift, &left);

  QCOMPARE(this->mOptions->operationsCount(), 4);

  QCOMPARE(mask, (quint32)0xab154e46);
  QCOMPARE(shift, 7);
  QCOMPARE(left, false);
}

void TestMatrixOptions::cleanupTestCase() { delete this->mOptions; }
