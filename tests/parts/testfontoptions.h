#ifndef TESTFONTOPTIONS_H
#define TESTFONTOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

class FontOptions;

class TestFontOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestFontOptions(QObject *parent = 0);
  virtual ~TestFontOptions() {}

private:
  FontOptions *mOptions;

private slots:
  void initTestCase();

  void bom();
  void encoding();

  void cleanupTestCase();
};

#endif // TESTFONTOPTIONS_H
