#include "testsettings.h"
#include <QDir>
#include <QDomDocument>
#include <QDomNodeList>
#include <QFile>
#include <QTemporaryDir>

TestSettings::TestSettings(QObject *parent) :
  QObject(parent)
{
}

const QString TestSettings::getFilename(const QTemporaryDir &dir) const
{
  return QDir::cleanPath(dir.path() + QDir::separator() + "config.xml");
}

void TestSettings::initTestCase()
{
}

void TestSettings::save()
{
  QTemporaryDir tempDir;

  if (tempDir.isValid()) {
    QString filename = this->getFilename(tempDir);

    // Save settings
    {
      AppSettings::configure(AppSettings::Section::Application, filename);
      AppSettings appsett(AppSettings::Section::Application);
      QSettings &sett = appsett.get();

      sett.setValue("section1/key1", "value1");
      sett.setValue("section1/key2", "value2");
      sett.setValue("section2/key10", "value10");
      sett.setValue("section2/key21", "value21");
      sett.setValue("section1/key4", "value1");
      sett.setValue("section1/key3", "value2");
      sett.setValue("section2/key5", "value10");
      sett.setValue("section2/key1", "value21");
    }

    // Load XML document
    {
      QFile file(filename);

      if (file.open(QIODevice::ReadOnly)) {
        QDomDocument doc;
        QString errorMsg;
        int errorColumn, errorLine;

        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
          QDomElement root = doc.documentElement();
          QCOMPARE(root.tagName(), QString("configuration"));
          QDomNodeList nodes = root.childNodes();
          QCOMPARE(nodes.count(), 2);
        } else {
          QFAIL("Cannot read XML content");
        }

        file.close();
      } else {
        QFAIL("File not opened");
      }
    }

  } else {
    QFAIL("Temp directory invalid");
  }
}

void TestSettings::load()
{
  QTemporaryDir tempDir;

  if (tempDir.isValid()) {
    QString content;
    QString filename = this->getFilename(tempDir);

    // Read from resources
    QFile resFile(":/settings/xml");

    if (resFile.open(QIODevice::ReadOnly)) {
      QTextStream stream(&resFile);
      content = stream.readAll();
      resFile.close();
    }

    // Save to file
    QFile tempFile(filename);

    if (tempFile.open(QIODevice::WriteOnly)) {
      QTextStream stream(&tempFile);
      stream << content;
      tempFile.close();
    }

    // Load settings
    {
      AppSettings::configure(AppSettings::Section::Application, filename);
      AppSettings appsett(AppSettings::Section::Application);
      QSettings &sett = appsett.get();

      QCOMPARE(sett.value("section1/key1").toString(), QString("value1"));
      QCOMPARE(sett.value("section1/key2").toString(), QString("value2"));
      QCOMPARE(sett.value("section2/key10").toString(), QString("value10"));
      QCOMPARE(sett.value("section2/key21").toString(), QString("value21"));
      QCOMPARE(sett.value("section1/key4").toString(), QString("value1"));
      QCOMPARE(sett.value("section1/key3").toString(), QString("value2"));
      QCOMPARE(sett.value("section2/key5").toString(), QString("value10"));
      QCOMPARE(sett.value("section2/key1").toString(), QString("value21"));
    }
  }
}

void TestSettings::save_load()
{
  QTemporaryDir tempDir;

  if (tempDir.isValid()) {
    // Set filename
    {
      QString filename = this->getFilename(tempDir);
      AppSettings::configure(AppSettings::Section::Application, filename);
    }

    QSettings::SettingsMap map;
    map.insert("section1/key1", 3.1415926);
    map.insert("section1/key2", 0x12345678);
    map.insert("section2/key10", 0x98765432);
    map.insert("section2/key21", -1);
    map.insert("section1/key4", 1.3e20);
    map.insert("section1/key3", "string");
    map.insert("section2/key5", "< > & \" \'");
    map.insert("section2/sub1/key1", "абвгдежз");

    // Save settings
    {
      AppSettings appsett(AppSettings::Section::Application);
      QSettings &sett = appsett.get();
      QMapIterator<QString, QVariant> it(map);

      while (it.hasNext()) {
        it.next();
        QString key = it.key();
        QVariant value = it.value();
        sett.setValue(key, value);
      }
    }

    // Load settings
    {
      AppSettings appsett(AppSettings::Section::Application);
      QSettings &sett = appsett.get();
      QMapIterator<QString, QVariant> it(map);

      while (it.hasNext()) {
        it.next();
        QString key = it.key();
        QVariant valueExpected = it.value();
        QVariant valueReal = sett.value(key);
        QCOMPARE(valueReal, valueExpected);
      }
    }
  }
}

void TestSettings::isNameStartCharValid()
{
  QStringList valid, invalid;
  valid << "tag" << "_tag" << "tag1";
  invalid << " " << "-tag" << "1tag" << "1" << "&";

  foreach (const QString &value, valid) {
    QCOMPARE(AppSettingsExt::isNameStartCharValid(value), true);
    QCOMPARE(AppSettingsExt::isNameCharValid(value), true);
  }

  foreach (const QString &value, invalid) {
    QCOMPARE(AppSettingsExt::isNameStartCharValid(value), false);
    QCOMPARE(AppSettingsExt::isNameCharValid(value), false);
  }
}

void TestSettings::escape()
{
  QStringList valid, invalid;
  valid << "tag" << "_tag" << "tag1" << "1tag" << "1" << "-tag";
  invalid << " " << "&" << "tag tag";

  foreach (const QString &value, valid) {
    QString escaped;
    bool success = AppSettingsExt::escape(value, escaped);
    QCOMPARE(success, true);
    QCOMPARE(AppSettingsExt::isNameCharValid(escaped), true);

    QString unescaped = AppSettingsExt::unescape(escaped);
    QCOMPARE(unescaped, value);
  }

  foreach (const QString &value, invalid) {
    QString escaped;
    bool success = AppSettingsExt::escape(value, escaped);
    QCOMPARE(success, false);
  }
}

void TestSettings::cleanupTestCase()
{
}
