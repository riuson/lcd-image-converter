#include "testbitstream.h"
//-----------------------------------------------------------------------------
TestBitStream::TestBitStream(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestBitStream::initTestCase()
{
    this->mMatrix = new ConversionMatrix(this);
    this->mMatrix->initColor(5, 6, 5);
    this->mMatrix->operationsRemoveAll();
    this->mMatrix->options()->setMaskAnd(0xffffffff);
    this->mMatrix->options()->setMaskOr(0x00000000);

    this->mMatrix->options()->setBlockSize(Data32);
}
//-----------------------------------------------------------------------------
void TestBitStream::streaming()
{
    this->mMatrix->options()->setMaskUsed(0x00ffffff);
    this->mMatrix->options()->setMaskFill(0xffffffff);

    // fill source data
    QVector<quint32> source, expected;
    this->preparePackData(
                0x00ffffff, 0xffffffff,
                &source, &expected);

    // create test data
    QVector<quint32> sample;
    BitStream stream(this->mMatrix, &source, 0, source.size());
    while (!stream.eof())
    {
        sample << stream.next();
    }

    // compare
    QCOMPARE(sample.count(), expected.count());
    for (int i = 0; i < sample.count(); i++)
    {
        QCOMPARE(sample.at(i), expected.at(i));
    }
}
//-----------------------------------------------------------------------------
void TestBitStream::cleanupTestCase()
{
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
void TestBitStream::preparePackData(
        quint32 maskUsed, quint32 maskFill,
        QVector<quint32> *source, QVector<quint32> *packed)
{
    const int count = 1000;
    qsrand(QTime::currentTime().msec());

    // fill source data
    for (int i = 0; i < count; i++)
    {
        quint32 value = qrand();
        value = 0x00ffff11;
        source->append(value);
    }

    QQueue<bool> bits;

    // stream row bits
    for (int i = 0; i < count; i++)
    {
        quint32 value = source->at(i);

        for (int j = 0; j < 32; j++)
        {
            if ((maskUsed & (0x80000000 >> j)) != 0)
            {
                bits.enqueue((value & (0x80000000 >> j)) != 0);
            }
        }
    }

    // pack bits
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
    }
}
//-----------------------------------------------------------------------------
