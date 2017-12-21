#include "testfontoptions.h"

#include <QStringList>
#include "fontoptions.h"

TestFontOptions::TestFontOptions(QObject *parent) :
  QObject(parent)
{
}

void TestFontOptions::initTestCase()
{
  this->mOptions = new Settings::Presets::FontOptions(this);
}

void TestFontOptions::bom()
{
  // 1
  this->mOptions->setBom(false);
  QCOMPARE(this->mOptions->bom(), false);

  // 2
  this->mOptions->setBom(true);
  QCOMPARE(this->mOptions->bom(), true);
}

void TestFontOptions::encoding()
{
  QStringList list = Settings::Presets::FontOptions::encodings();

  for (int i = 0; i < list.length(); i++) {
    this->mOptions->setEncoding(list.at(i));
    QCOMPARE(this->mOptions->encoding(), list.at(i));
  }

  this->mOptions->setEncoding("other");
  QVERIFY(this->mOptions->encoding() != "other");
}

void TestFontOptions::cleanupTestCase()
{
  delete this->mOptions;
}

