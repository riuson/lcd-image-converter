#ifndef TESTPRESETS_H
#define TESTPRESETS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class Preset;
}
}

class TestPresets : public QObject
{
  Q_OBJECT
public:
  explicit TestPresets(QObject *parent = 0);
  virtual ~TestPresets() {}

private:
  Settings::Presets::Preset *mOptions;

private slots:
  void initTestCase();

  void loadSave();

  void cleanupTestCase();
};

#endif // TESTPRESETS_H
