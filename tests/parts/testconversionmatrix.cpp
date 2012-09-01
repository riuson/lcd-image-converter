#include "testconversionmatrix.h"
//-----------------------------------------------------------------------------
TestConversionMatrix::TestConversionMatrix(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::initTestCase()
{
    this->mMatrix = new ConversionMatrix(this);
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationsCount()
{
    this->mMatrix->operationsRemoveAll();

    this->mMatrix->operationAdd(0x12345678, 0, false);
    QCOMPARE(this->mMatrix->operationsCount(), 1);

    this->mMatrix->operationAdd(0x23131315, 1, true);
    QCOMPARE(this->mMatrix->operationsCount(), 2);
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operation()
{
    this->mMatrix->operationsRemoveAll();

    quint32 expectedMask = 0x12345678;
    int expectedShift = 10;
    bool expectedLeft = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(expectedMask, expectedShift, expectedLeft);

        quint32 actualMask;
        int actualShift;
        bool actualLeft;
        this->mMatrix->operation(i, &actualMask, &actualShift, &actualLeft);

        if (expectedShift >= 0)
        {
            QCOMPARE(actualMask, expectedMask);
            QCOMPARE(actualShift, expectedShift);
            QCOMPARE(actualLeft, expectedLeft);
        }
        else
        {
            QCOMPARE(actualMask, expectedMask);
            QVERIFY(actualShift >= 0);
            QCOMPARE(actualLeft, expectedLeft);
        }

        expectedMask = expectedMask >> 3;
        expectedShift--;
        expectedLeft = !expectedLeft;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationAdd()
{
    this->mMatrix->operationsRemoveAll();

    quint32 expectedMask = 0x41654649;
    int expectedShift = 10;
    bool expectedLeft = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(expectedMask, expectedShift, expectedLeft);

        quint32 actualMask;
        int actualShift;
        bool actualLeft;
        this->mMatrix->operation(i, &actualMask, &actualShift, &actualLeft);

        if (expectedShift >= 0)
        {
            QCOMPARE(actualMask, expectedMask);
            QCOMPARE(actualShift, expectedShift);
            QCOMPARE(actualLeft, expectedLeft);
        }
        else
        {
            QCOMPARE(actualMask, expectedMask);
            QVERIFY(actualShift >= 0);
            QCOMPARE(actualLeft, expectedLeft);
        }

        expectedMask = expectedMask << 1;
        expectedShift--;
        expectedLeft = !expectedLeft;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationRemove()
{
    this->mMatrix->operationsRemoveAll();

    this->mMatrix->operationAdd(0x12345678, 5, false);
    this->mMatrix->operationAdd(0x87654321, 3, true);

    quint32 actualMask;
    int actualShift;
    bool actualLeft;

    this->mMatrix->operationRemove(0);

    this->mMatrix->operation(0, &actualMask, &actualShift, &actualLeft);

    if (this->mMatrix->operationsCount() != 1)
        QFAIL("operationRemove failed");

    QCOMPARE(actualMask, 0x87654321);
    QCOMPARE(actualShift, 3);
    QCOMPARE(actualLeft, true);
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationRemoveAll()
{
    this->mMatrix->operationsRemoveAll();

    quint32 mask = 0x41654649;
    int shift = 10;
    bool left = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(mask, shift, left);
    }
    QCOMPARE(this->mMatrix->operationsCount(), 20);

    this->mMatrix->operationsRemoveAll();
    QCOMPARE(this->mMatrix->operationsCount(), 0);
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationReplace()
{
    this->mMatrix->operationsRemoveAll();

    this->mMatrix->operationAdd(0x12345678, 5, false);
    this->mMatrix->operationAdd(0x87654321, 3, true);
    this->mMatrix->operationAdd(0x98abcdef, 7, false);
    this->mMatrix->operationAdd(0xdf546b01, 2, true);

    QCOMPARE(this->mMatrix->operationsCount(), 4);

    quint32 mask;
    int shift;
    bool left;

    // 1
    this->mMatrix->operationReplace(2, 0x1234f00d, 0, true);

    this->mMatrix->operation(2, &mask, &shift, &left);

    QCOMPARE(this->mMatrix->operationsCount(), 4);

    QCOMPARE(mask, (quint32)0x1234f00d);
    QCOMPARE(shift, 0);
    QCOMPARE(left, true);

    // 2
    this->mMatrix->operationReplace(2, 0xab154e46, 7, false);

    this->mMatrix->operation(2, &mask, &shift, &left);

    QCOMPARE(this->mMatrix->operationsCount(), 4);

    QCOMPARE(mask, (quint32)0xab154e46);
    QCOMPARE(shift, 7);
    QCOMPARE(left, false);
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::loadSave()
{
    ConversionMatrix sourceMatrix(this);
    sourceMatrix.initColor(5, 6, 5);
    sourceMatrix.options()->setBytesOrder(BytesOrderBigEndian);
    sourceMatrix.options()->setFlipVertical(true);
    sourceMatrix.options()->setRotate(Rotate270);

    this->mMatrix->operationsRemoveAll();
    this->mMatrix->initColor(5, 6, 5);
    this->mMatrix->options()->setBytesOrder(BytesOrderBigEndian);
    this->mMatrix->options()->setFlipVertical(true);
    this->mMatrix->options()->setRotate(Rotate270);

    for (int i = 0; i < 10; i++)
    {
        this->mMatrix->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
        sourceMatrix.operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
    }

    const QString name = "testconversionmatrix";
    if (!this->mMatrix->save(name))
        QFAIL("preset not saved");

    this->mMatrix->operationsRemoveAll();
    this->mMatrix->initMono(MonochromeTypeEdge, 1);

    if (!this->mMatrix->load(name))
        QFAIL("preset not loaded");

    QCOMPARE(this->mMatrix->options()->bytesOrder(),     sourceMatrix.options()->bytesOrder());
    QCOMPARE(this->mMatrix->options()->convType(),       sourceMatrix.options()->convType());
    QCOMPARE(this->mMatrix->options()->monoType(),       sourceMatrix.options()->monoType());
    QCOMPARE(this->mMatrix->options()->edge(),           sourceMatrix.options()->edge());
    QCOMPARE(this->mMatrix->options()->blockSize(),      sourceMatrix.options()->blockSize());
    QCOMPARE(this->mMatrix->options()->rotate(),         sourceMatrix.options()->rotate());
    QCOMPARE(this->mMatrix->options()->flipVertical(),   sourceMatrix.options()->flipVertical());
    QCOMPARE(this->mMatrix->options()->flipHorizontal(), sourceMatrix.options()->flipHorizontal());
    QCOMPARE(this->mMatrix->options()->inverse(),        sourceMatrix.options()->inverse());
    QCOMPARE(this->mMatrix->options()->maskUsed(),       sourceMatrix.options()->maskUsed());
    QCOMPARE(this->mMatrix->options()->maskAnd(),        sourceMatrix.options()->maskAnd());
    QCOMPARE(this->mMatrix->options()->maskOr(),         sourceMatrix.options()->maskOr());
    QCOMPARE(this->mMatrix->options()->maskFill(),       sourceMatrix.options()->maskFill());
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::cleanupTestCase()
{
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
