#ifndef TESTFONTOPTIONS_H
#define TESTFONTOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class FontOptions;
}
} // namespace Settings

class TestFontOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestFontOptions(QObject* parent = 0);
  virtual ~TestFontOptions() {}

private:
  Settings::Presets::FontOptions* mOptions;

private slots:
  void initTestCase();

  void bom();
  void encoding();

  void cleanupTestCase();
};

#endif // TESTFONTOPTIONS_H
