#include "testbitstream.h"

#include <QRandomGenerator>
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"

TestBitStream::TestBitStream(QObject *parent) :
  QObject(parent)
{
}

void TestBitStream::initTestCase()
{
  this->mPreset = new Settings::Presets::Preset(this);
  this->mPreset->initColor(0, 5, 6, 5);
  this->mPreset->matrix()->operationsRemoveAll();
  this->mPreset->matrix()->setMaskAnd(0xffffffff);
  this->mPreset->matrix()->setMaskOr(0x00000000);

  this->mPreset->image()->setBlockSize(Settings::Presets::DataBlockSize::Data32);
}

void TestBitStream::streaming()
{
  this->mPreset->matrix()->setMaskUsed(0x00ffffff);
  this->mPreset->matrix()->setMaskFill(0xffffffff);

  // fill source data
  QVector<quint32> source, expected;
  this->preparePackData(
    0x00ffffff, 0xffffffff,
    &source, &expected);

  // create test data
  QVector<quint32> sample;
  Parsing::Conversion::BitStream stream(this->mPreset, &source, 0, source.size());

  while (!stream.eof()) {
    sample << stream.next();
  }

  // compare
  QCOMPARE(sample.count(), expected.count());

  for (int i = 0; i < sample.count(); i++) {
    QCOMPARE(sample.at(i), expected.at(i));
  }
}

void TestBitStream::cleanupTestCase()
{
  delete this->mPreset;
}

void TestBitStream::preparePackData(
  quint32 maskUsed, quint32 maskFill,
  QVector<quint32> *source, QVector<quint32> *packed)
{
  const int count = 1000;
  QRandomGenerator prng(QTime::currentTime().msec());

  // fill source data
  for (int i = 0; i < count; i++) {
    quint32 value = prng.generate();
    value = 0x00ffff11;
    source->append(value);
  }

  QQueue<bool> bits;

  // stream row bits
  for (int i = 0; i < count; i++) {
    quint32 value = source->at(i);

    for (int j = 0; j < 32; j++) {
      if ((maskUsed & (0x80000000 >> j)) != 0) {
        bits.enqueue((value & (0x80000000 >> j)) != 0);
      }
    }
  }

  // pack bits
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
  }
}

