#ifndef TESTBITSTREAM_H
#define TESTBITSTREAM_H

#include <QObject>
#include <QtTest/QtTest>

#include "bitstream.h"

class TestBitStream : public QObject
{
  Q_OBJECT
public:
  explicit TestBitStream(QObject* parent = 0);
  virtual ~TestBitStream() {}

private:
  Settings::Presets::Preset* mPreset;
  void preparePackData(quint32 maskUsed, quint32 maskFill, QVector<quint32>* source, QVector<quint32>* packed);

private slots:
  void initTestCase();
  void streaming();
  void cleanupTestCase();
};

#endif // TESTBITSTREAM_H
