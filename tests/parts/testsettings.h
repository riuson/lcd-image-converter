#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QFile>
#include <QObject>
#include <QtTest/QtTest>

#include "appsettings.h"
template <class T> class QVector;

class AppSettingsExt : public Settings::AppSettings
{
public:
  using AppSettings::escape;
  using AppSettings::isNameCharValid;
  using AppSettings::isNameStartCharValid;
  using AppSettings::unescape;
};

class TestSettings : public QObject
{
  Q_OBJECT
public:
  explicit TestSettings(QObject* parent = 0);
  virtual ~TestSettings() {}

private:
  const QString getFilename(const QTemporaryDir& dir) const;
private slots:
  void initTestCase();

  void save();
  void load();
  void save_load();
  void isNameStartCharValid();
  void escape();

  void cleanupTestCase();
};

#endif // TESTSETTINGS_H
