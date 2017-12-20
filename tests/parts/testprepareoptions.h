#ifndef TESTPREPAREOPTIONS_H
#define TESTPREPAREOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class PrepareOptions;
}
}

class TestPrepareOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestPrepareOptions(QObject *parent = 0);
  virtual ~TestPrepareOptions() {}

private:
  Settings::Presets::PrepareOptions *mOptions;

private slots:
  void initTestCase();

  void convType();
  void monoType();
  void edge();
  void scanMain();
  void scanSub();
  void inverse();
  void bandScanning();
  void bandWidth();

  void cleanupTestCase();
};

#endif // TESTPREPAREOPTIONS_H
