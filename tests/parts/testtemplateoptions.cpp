#include "testtemplateoptions.h"

#include "templateoptions.h"

TestTemplateOptions::TestTemplateOptions(QObject *parent) :
    QObject(parent)
{
}

void TestTemplateOptions::initTestCase()
{
    this->mOptions = new TemplateOptions(this);
}

void TestTemplateOptions::image()
{
    // 1
    this->mOptions->setImage("test1");
    QCOMPARE(this->mOptions->image(), QString("test1"));

    // 2
    this->mOptions->setImage("test2");
    QCOMPARE(this->mOptions->image(), QString("test2"));
}

void TestTemplateOptions::font()
{
    // 1
    this->mOptions->setFont("test3");
    QCOMPARE(this->mOptions->font(), QString("test3"));

    // 2
    this->mOptions->setFont("test4");
    QCOMPARE(this->mOptions->font(), QString("test4"));
}

void TestTemplateOptions::cleanupTestCase()
{
    delete this->mOptions;
}

