#ifndef TESTCONVERTERHELPER_H
#define TESTCONVERTERHELPER_H

#include <QObject>
#include <QtTest/QtTest>
#include "conversion_options.h"
#include "convimagescan.h"

namespace Settings
{
namespace Presets
{
class Preset;
}
}

class TestConverterHelper : public QObject
{
  Q_OBJECT
public:
  explicit TestConverterHelper(QObject *parent = 0);
  virtual ~TestConverterHelper() {}

private:
  Settings::Presets::Preset *mPreset;
  void preparePackData(
    quint32 maskUsed, quint32 maskFill,
    QVector<quint32> *source, int width, int height,
    bool splitToRows, QVector<quint32> *packed, int *widthOut, int *heightOut);
  void prepareStringData(
    QVector<quint32> *source, int width, int height,
    bool splitToRows, Parsing::Conversion::Options::DataBlockSize size, QString *string);

private slots:
  void initTestCase();
  void processPixels();
  void packData();
  void dataToString();
  void uint2hex();
  void uint2octal();
  void uint2binary();
  void jsengineSetProperty();
  void breakInfiniteScript();
  void cleanupTestCase();
};

class TestConvImage : public Parsing::Conversion::ConvImageScan
{
  Q_OBJECT
public:
  enum Condition {
    CanBeDeleted,
    CannotBeDeleted
  };

  explicit TestConvImage(const QImage *image, QObject *parent = 0);
  virtual ~TestConvImage();

  void setCondition(Condition value);

private:
  Condition mCondition;
};

#endif // TESTCONVERTERHELPER_H
