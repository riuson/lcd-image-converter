#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QObject>
#include <QtTest/QtTest>
#include "appsettings.h"

class QFile;
template <class T> class QVector;

class AppSettingsExt : public AppSettings
{
public:
  using AppSettings::isNameStartCharValid;
  using AppSettings::isNameCharValid;
  using AppSettings::escape;
  using AppSettings::unescape;
};

class TestSettings : public QObject
{
  Q_OBJECT
public:
  explicit TestSettings(QObject *parent = 0);

private:
  const QString getFilename(const QTemporaryDir &dir) const;
private slots:
  void initTestCase();

  void save();
  void load();
  void save_load();
  void isNameStartCharValid();

  void cleanupTestCase();
};

#endif // TESTSETTINGS_H
