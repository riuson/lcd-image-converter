#ifndef TESTIMAGEOPTIONS_H
#define TESTIMAGEOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class ImageOptions;
}
}

class TestImageOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestImageOptions(QObject *parent = 0);
  virtual ~TestImageOptions() {}

private:
  Settings::Presets::ImageOptions *mOptions;

private slots:
  void initTestCase();

  void bytesOrder();
  void blockSize();
  void blockDefaultOnes();
  void splitToRows();
  void compressionRle();
  void blockPrefix();
  void blockSuffix();
  void blockDelimiter();

  void cleanupTestCase();
};

#endif // TESTIMAGEOPTIONS_H
