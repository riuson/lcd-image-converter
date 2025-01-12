#ifndef TESTREORDERINGOPTIONS_H
#define TESTREORDERINGOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

namespace Settings
{
namespace Presets
{
class ReorderingOptions;
}
} // namespace Settings

class TestReorderingOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestReorderingOptions(QObject* parent = 0);
  virtual ~TestReorderingOptions() {}

private:
  Settings::Presets::ReorderingOptions* mOptions;

private slots:
  void initTestCase();

  void operationsCount();
  void operation();
  void operationAdd();
  void operationRemove();
  void operationRemoveAll();
  void operationReplace();

  void cleanupTestCase();
};

#endif // TESTREORDERINGOPTIONS_H
