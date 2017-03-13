#include "testsettings.h"
#include "appsettings.h"
#include <QDir>
#include <QDomDocument>
#include <QDomNodeList>
#include <QFile>
#include <QTemporaryDir>

TestSettings::TestSettings(QObject *parent) :
  QObject(parent)
{

}

bool TestSettings::readFile(QFile &file, QString &result)
{
  if (file.isOpen()) {
    QTextStream stream(&file);
    stream.seek(0);
    result = stream.readAll();
    return true;
  }

  result = "Failure";
  return false;
}

void TestSettings::initTestCase()
{
}

void TestSettings::save()
{
  QTemporaryDir tempDir;

  if (tempDir.isValid()) {
    //tempDir.setAutoRemove(false);
    QString filename = QDir::cleanPath(tempDir.path() + QDir::separator() + "config.xml");

    //qDebug() << "Use file: " << filename;

    // Save settings
    {
      AppSettings::configure(filename);
      AppSettings appsett;
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
        //QString content;
        //this->readFile(file, content);
        //qDebug() << "Content: " << content;
        //file.seek(0);

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

    //tempfile.close();
  } else {
    QFAIL("Temp directory invalid");
  }
}

void TestSettings::load()
{
  QTemporaryDir tempDir;

  if (tempDir.isValid()) {
    QString content;
    QString filename = QDir::cleanPath(tempDir.path() + QDir::separator() + "config.xml");

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
      AppSettings::configure(filename);
      AppSettings appsett;
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

void TestSettings::cleanupTestCase()
{

}
