#ifndef TESTSETTINGS_H
#define TESTSETTINGS_H

#include <QObject>
#include <QtTest/QtTest>

class QFile;
template <class T> class QVector;

class TestSettings : public QObject
{
  Q_OBJECT
public:
  explicit TestSettings(QObject *parent = 0);

private:
  QString mFilename;

  bool readFile(QFile &file, QString &result);

private slots:
  void initTestCase();

  void save();
  void load();
  void save_load();

  void cleanupTestCase();
};

#endif // TESTSETTINGS_H
