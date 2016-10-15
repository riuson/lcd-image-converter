#ifndef TESTPRESETS_H
#define TESTPRESETS_H

#include <QObject>
#include <QtTest/QtTest>

class Preset;

class TestPresets : public QObject
{
  Q_OBJECT
public:
  explicit TestPresets(QObject *parent = 0);

private:
  Preset *mOptions;

private slots:
  void initTestCase();

  void loadSave();

  void cleanupTestCase();
};

#endif // TESTPRESETS_H
