#include "testprepareoptions.h"
//-----------------------------------------------------------------------------
#include "prepareoptions.h"
//-----------------------------------------------------------------------------
TestPrepareOptions::TestPrepareOptions(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::initTestCase()
{
    this->mOptions = new PrepareOptions(this);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::convType()
{
    // 1
    this->mOptions->setConvType(ConversionTypeMonochrome);
    QCOMPARE(this->mOptions->convType(), ConversionTypeMonochrome);

    // 2
    this->mOptions->setConvType(ConversionTypeGrayscale);
    QCOMPARE(this->mOptions->convType(), ConversionTypeGrayscale);

    // 3
    this->mOptions->setConvType(ConversionTypeColor);
    QCOMPARE(this->mOptions->convType(), ConversionTypeColor);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::monoType()
{
    // 1
    this->mOptions->setMonoType(MonochromeTypeEdge);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeEdge);

    // 2
    this->mOptions->setMonoType(MonochromeTypeDiffuseDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeDiffuseDither);

    // 3
    this->mOptions->setMonoType(MonochromeTypeOrderedDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeOrderedDither);

    // 4
    this->mOptions->setMonoType(MonochromeTypeThresholdDither);
    QCOMPARE(this->mOptions->monoType(), MonochromeTypeThresholdDither);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::edge()
{
    for (int i = -512; i < 512; i++)
    {
        this->mOptions->setEdge(i);
        if (i >= 0 && i <= 255)
        {
            QCOMPARE(this->mOptions->edge(), i);
        }
        else
        {
            QVERIFY(this->mOptions->edge() >= 0);
            QVERIFY(this->mOptions->edge() <= 255);
        }
    }
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::rotate()
{
    // 1
    this->mOptions->setRotate(RotateNone);
    QCOMPARE(this->mOptions->rotate(), RotateNone);

    // 2
    this->mOptions->setRotate(Rotate90);
    QCOMPARE(this->mOptions->rotate(), Rotate90);

    // 3
    this->mOptions->setRotate(Rotate180);
    QCOMPARE(this->mOptions->rotate(), Rotate180);

    // 4
    this->mOptions->setRotate(Rotate270);
    QCOMPARE(this->mOptions->rotate(), Rotate270);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::flipVertical()
{
    // 1
    this->mOptions->setFlipVertical(false);
    QCOMPARE(this->mOptions->flipVertical(), false);

    // 2
    this->mOptions->setFlipVertical(true);
    QCOMPARE(this->mOptions->flipVertical(), true);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::flipHorizontal()
{
    // 1
    this->mOptions->setFlipHorizontal(false);
    QCOMPARE(this->mOptions->flipHorizontal(), false);

    // 2
    this->mOptions->setFlipHorizontal(true);
    QCOMPARE(this->mOptions->flipHorizontal(), true);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::inverse()
{
    // 1
    this->mOptions->setInverse(false);
    QCOMPARE(this->mOptions->inverse(), false);

    // 2
    this->mOptions->setInverse(true);
    QCOMPARE(this->mOptions->inverse(), true);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
