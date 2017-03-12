#include "testsettings.h"
#include "appsettings.h"
#include <QDir>
#include <QDomDocument>
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

    qDebug() << "Use file: " << filename;

    // Save settings
    {
      AppSettings::configure(filename);
      AppSettings appsett;
      QSettings &sett = appsett.get();

      sett.beginGroup("section");
      sett.setValue("key1", "value1");
      sett.setValue("key2", "value2");
      sett.endGroup();
    }

    // Load XML document
    {
      QFile file(filename);

      if (file.open(QIODevice::ReadOnly)) {
        QString content;
        this->readFile(file, content);
        qDebug() << "Content: " << content;
        file.seek(0);

        QDomDocument doc;
        QString errorMsg;
        int errorColumn, errorLine;

        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
          QDomElement root = doc.documentElement();
          QCOMPARE(root.tagName(), QString("configuration"));

          if (root.tagName() == "configuration") {
          }
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

void TestSettings::saveAndLoad()
{

}

void TestSettings::cleanupTestCase()
{

}
