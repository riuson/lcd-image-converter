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
    if (this->mMatrix->operationsCount() != 1)
        QFAIL("operationsCount failed");

    this->mMatrix->operationAdd(0x23131315, 1, true);
    if (this->mMatrix->operationsCount() != 2)
        QFAIL("operationsCount failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operation()
{
    this->mMatrix->operationsRemoveAll();

    quint32 srcMask = 0x12345678;
    int srcShift = 10;
    bool srcLeft = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(srcMask, srcShift, srcLeft);

        quint32 mask;
        int shift;
        bool left;
        this->mMatrix->operation(i, &mask, &shift, &left);

        if (srcShift >= 0)
        {
            if ((mask != srcMask) || (shift != srcShift) || (left != srcLeft))
                QFAIL("operation failed");
        }
        else
        {
            if ((mask != srcMask) || (shift < 0) || (left != srcLeft))
                QFAIL("operation failed");
        }

        srcMask = srcMask >> 3;
        srcShift--;
        srcLeft = !srcLeft;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationAdd()
{
    this->mMatrix->operationsRemoveAll();

    quint32 srcMask = 0x41654649;
    int srcShift = 10;
    bool srcLeft = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(srcMask, srcShift, srcLeft);

        quint32 mask;
        int shift;
        bool left;
        this->mMatrix->operation(i, &mask, &shift, &left);

        if (srcShift >= 0)
        {
            if ((mask != srcMask) || (shift != srcShift) || (left != srcLeft))
                QFAIL("operation failed");
        }
        else
        {
            if ((mask != srcMask) || (shift < 0) || (left != srcLeft))
                QFAIL("operation failed");
        }

        srcMask = srcMask << 1;
        srcShift--;
        srcLeft = !srcLeft;
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationRemove()
{
    this->mMatrix->operationsRemoveAll();

    this->mMatrix->operationAdd(0x12345678, 5, false);
    this->mMatrix->operationAdd(0x87654321, 3, true);

    quint32 mask;
    int shift;
    bool left;

    this->mMatrix->operationRemove(0);

    this->mMatrix->operation(0, &mask, &shift, &left);

    if (this->mMatrix->operationsCount() != 1)
        QFAIL("operationRemove failed");

    if ((mask != 0x87654321) || (shift != 3) || (left != true))
        QFAIL("operationRemove failed");
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationRemoveAll()
{
    this->mMatrix->operationsRemoveAll();

    quint32 srcMask = 0x41654649;
    int srcShift = 10;
    bool srcLeft = true;

    for (int i = 0; i < 20; i++)
    {
        this->mMatrix->operationAdd(srcMask, srcShift, srcLeft);
    }

    if (this->mMatrix->operationsCount() == 20)
    {
        this->mMatrix->operationsRemoveAll();
        if (this->mMatrix->operationsCount() != 0)
        {
            QFAIL("not removed");
        }
    }
    else
    {
        QFAIL("No operations added");
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::operationReplace()
{
    this->mMatrix->operationsRemoveAll();

    this->mMatrix->operationAdd(0x12345678, 5, false);
    this->mMatrix->operationAdd(0x87654321, 3, true);
    this->mMatrix->operationAdd(0x98abcdef, 7, false);
    this->mMatrix->operationAdd(0xdf546b01, 2, true);

    quint32 mask;
    int shift;
    bool left;

    // 1
    this->mMatrix->operationReplace(2, 0x1234f00d, 0, true);

    this->mMatrix->operation(2, &mask, &shift, &left);

    if (this->mMatrix->operationsCount() != 4)
        QFAIL("operationReplace failed");

    if ((mask != 0x1234f00d) || (shift != 0) || (left != true))
        QFAIL("operationReplace failed");

    // 2
    this->mMatrix->operationReplace(2, 0xab154e46, 7, false);

    this->mMatrix->operation(2, &mask, &shift, &left);

    if (this->mMatrix->operationsCount() != 4)
        QFAIL("operationReplace failed");

    if ((mask != 0xab154e46) || (shift != 7) || (left != false))
        QFAIL("operationReplace failed");
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

    if (
            (this->mMatrix->options()->bytesOrder() != sourceMatrix.options()->bytesOrder()) ||
            (this->mMatrix->options()->convType() != sourceMatrix.options()->convType()) ||
            (this->mMatrix->options()->monoType() != sourceMatrix.options()->monoType()) ||
            (this->mMatrix->options()->edge() != sourceMatrix.options()->edge()) ||
            (this->mMatrix->options()->blockSize() != sourceMatrix.options()->blockSize()) ||
            (this->mMatrix->options()->rotate() != sourceMatrix.options()->rotate()) ||
            (this->mMatrix->options()->flipVertical() != sourceMatrix.options()->flipVertical()) ||
            (this->mMatrix->options()->flipHorizontal() != sourceMatrix.options()->flipHorizontal()) ||
            (this->mMatrix->options()->inverse() != sourceMatrix.options()->inverse()) ||
            (this->mMatrix->options()->maskUsed() != sourceMatrix.options()->maskUsed()) ||
            (this->mMatrix->options()->maskAnd() != sourceMatrix.options()->maskAnd()) ||
            (this->mMatrix->options()->maskOr() != sourceMatrix.options()->maskOr()) ||
            (this->mMatrix->options()->maskFill() != sourceMatrix.options()->maskFill())
            )
    {
        QFAIL("loaded preset not match");
    }
}
//-----------------------------------------------------------------------------
void TestConversionMatrix::cleanupTestCase()
{
    delete this->mMatrix;
}
//-----------------------------------------------------------------------------
