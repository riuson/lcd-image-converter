#ifndef TESTTEMPLATEOPTIONS_H
#define TESTTEMPLATEOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class TemplateOptions;
}
} // namespace Settings

class TestTemplateOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestTemplateOptions(QObject* parent = 0);
  virtual ~TestTemplateOptions() {}

private:
  Settings::Presets::TemplateOptions* mOptions;

private slots:
  void initTestCase();

  void image();
  void font();

  void cleanupTestCase();
};

#endif // TESTTEMPLATEOPTIONS_H
