#include "testfontoptions.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include "fontoptions.h"
//-----------------------------------------------------------------------------
TestFontOptions::TestFontOptions(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestFontOptions::initTestCase()
{
    this->mOptions = new FontOptions(this);
}
//-----------------------------------------------------------------------------
void TestFontOptions::bom()
{
    // 1
    this->mOptions->setBom(false);
    QCOMPARE(this->mOptions->bom(), false);

    // 2
    this->mOptions->setBom(true);
    QCOMPARE(this->mOptions->bom(), true);
}
//-----------------------------------------------------------------------------
void TestFontOptions::encoding()
{
    QStringList list = FontOptions::codecs();
    for (int i = 0; i < list.length(); i++)
    {
        this->mOptions->setCodec(list.at(i));
        QCOMPARE(this->mOptions->codec(), list.at(i));
    }
    this->mOptions->setCodec("other");
    QVERIFY(this->mOptions->codec() != "other");
}
//-----------------------------------------------------------------------------
void TestFontOptions::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
