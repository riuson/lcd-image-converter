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
void TestConverterHelper::packData()
{
    const int count = 1000;
    this->mMatrix->initColor(5, 6, 5);
    this->mMatrix->operationsRemoveAll();
    this->mMatrix->options()->setMaskAnd(0xffffffff);
    this->mMatrix->options()->setMaskOr(0x00000000);

    this->mMatrix->options()->setBlockSize(Data32);
    this->mMatrix->options()->setMaskUsed(0x00ffffff);
    this->mMatrix->options()->setMaskFill(0xffffffff);

    QVector<quint32> source, expected;
    int widthExpected, heightExpected;
    this->preparePackData(
                0x00ffffff, 0xffffffff,
                &source, 1000, 1000,
                &expected, &widthExpected, &heightExpected);

    QVector<quint32> sample;
    int widthSample, heightSample;
    ConverterHelper::packData(this->mMatrix, &source, 1000, 1000, &sample, &widthSample, &heightSample);

    QCOMPARE(widthSample, widthExpected);
    QCOMPARE(heightSample, heightExpected);
    QCOMPARE(sample.count(), expected.count());

    for (int i = 0; i < sample.count(); i++)
    {
        QCOMPARE(sample.at(i), expected.at(i));
    }
}
//-----------------------------------------------------------------------------
void TestConverterHelper::cleanupTestCase()
{
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
void TestConverterHelper::preparePackData(
        quint32 maskUsed, quint32 maskFill,
        QVector<quint32> *source, int width, int height,
        QVector<quint32> *packed, int *widthOut, int *heightOut)
{
    int packedRowWidth = 0;

    qsrand(QTime::currentTime().msec());

    // fill source data
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            quint32 value = qrand();
            value = 0x00ffff11;
            source->append(value);
        }
    }

    for (int y = 0; y < height; y++)
    {
        QQueue<bool> bits;

        // stream row bits
        for (int x = 0; x < width; x++)
        {
            quint32 value = source->at(x + width * y);

            for (int j = 0; j < 32; j++)
            {
                if ((maskUsed & (0x80000000 >> j)) != 0)
                {
                    bits.enqueue((value & (0x80000000 >> j)) != 0);
                }
            }
        }

        // pack bits
        int counter = 0;
        while (!bits.empty())
        {
            quint32 value = 0;
            for (int j = 0; j < 32 && !bits.empty(); j++)
            {
                if ((maskFill & (0x80000000 >> j)) != 0)
                {
                    bool bit = bits.dequeue();
                    if (bit)
                    {
                        value |= (0x80000000 >> j);
                    }
                }
            }
            packed->append(value);
            counter++;
        }
        packedRowWidth = qMax(packedRowWidth, counter);
    }

    *widthOut = packedRowWidth;
    *heightOut = height;
}
//-----------------------------------------------------------------------------
