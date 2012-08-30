#include "testconverterhelper.h"

#include <QVector>
#include "converterhelper.h"
//-----------------------------------------------------------------------------
TestConverterHelper::TestConverterHelper(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestConverterHelper::initTestCase()
{
    this->mMatrix = new ConversionMatrix(this);
}
//-----------------------------------------------------------------------------
void TestConverterHelper::processPixels()
{
    const int count = 1000;
    this->mMatrix->initColor(5, 6, 5);
    this->mMatrix->operationsRemoveAll();
    this->mMatrix->options()->setBlockSize(Data32);
    this->mMatrix->options()->setMaskUsed(0x00ffffff);
    this->mMatrix->options()->setMaskAnd(0xffffffff);
    this->mMatrix->options()->setMaskOr(0x00000000);
    this->mMatrix->options()->setMaskFill(0xffffffff);

    // equals by default
    QVector<quint32> source, sample;
    qsrand(QTime::currentTime().msec());
    for (int i = 0; i < count; i++)
    {
        quint32 value = qrand();
        source << value;
        sample << value;
    }

    // no operations, no changes
    ConverterHelper::processPixels(this->mMatrix, &sample);
    for (int i = 0; i < count; i++)
    {
        QCOMPARE(sample.at(i), source.at(i));
    }

    // add operations
    this->mMatrix->operationAdd(0x12345678, 3, true);
    this->mMatrix->operationAdd(0x87654321, 1, false);
    source.clear();
    sample.clear();
    for (int i = 0; i < count; i++)
    {
        quint32 value = qrand();
        quint32 a = 0;
        a |= (value & 0x12345678) << 3;
        a |= (value & 0x87654321) >> 1;
        source << a;
        sample << value;
    }
    ConverterHelper::processPixels(this->mMatrix, &sample);
    for (int i = 0; i < count; i++)
    {
        QCOMPARE(sample.at(i), source.at(i));
    }

    // test AND mask
    this->mMatrix->operationsRemoveAll();
    this->mMatrix->options()->setMaskAnd(0xabcdef01);
    this->mMatrix->options()->setMaskOr(0x00000000);
    source.clear();
    sample.clear();
    for (int i = 0; i < count; i++)
    {
        quint32 value = qrand();
        quint32 a = value & 0xabcdef01;
        source << a;
        sample << value;
    }
    ConverterHelper::processPixels(this->mMatrix, &sample);
    for (int i = 0; i < count; i++)
    {
        QCOMPARE(sample.at(i), source.at(i));
    }

    // test OR mask
    this->mMatrix->operationsRemoveAll();
    this->mMatrix->options()->setMaskAnd(0xffffffff);
    this->mMatrix->options()->setMaskOr(0x1a5b5e4d);
    source.clear();
    sample.clear();
    for (int i = 0; i < count; i++)
    {
        quint32 value = qrand();
        quint32 a = value | 0x1a5b5e4d;
        source << a;
        sample << value;
    }
    ConverterHelper::processPixels(this->mMatrix, &sample);
    for (int i = 0; i < count; i++)
    {
        QCOMPARE(sample.at(i), source.at(i));
    }
}
//-----------------------------------------------------------------------------
void TestConverterHelper::cleanupTestCase()
{
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
