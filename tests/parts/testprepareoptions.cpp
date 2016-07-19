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
void TestPrepareOptions::scanMain()
{
    // 1
    this->mOptions->setScanMain(TopToBottom);
    QCOMPARE(this->mOptions->scanMain(), TopToBottom);

    // 2
    this->mOptions->setScanMain(BottomToTop);
    QCOMPARE(this->mOptions->scanMain(), BottomToTop);

    // 3
    this->mOptions->setScanMain(LeftToRight);
    QCOMPARE(this->mOptions->scanMain(), LeftToRight);

    // 4
    this->mOptions->setScanMain(RightToLeft);
    QCOMPARE(this->mOptions->scanMain(), RightToLeft);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::scanSub()
{
    // 1
    this->mOptions->setScanSub(Forward);
    QCOMPARE(this->mOptions->scanSub(), Forward);

    // 2
    this->mOptions->setScanSub(Backward);
    QCOMPARE(this->mOptions->scanSub(), Backward);
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
void TestPrepareOptions::bandScanning()
{
    // 1
    this->mOptions->setBandScanning(false);
    QCOMPARE(this->mOptions->bandScanning(), false);

    // 2
    this->mOptions->setBandScanning(true);
    QCOMPARE(this->mOptions->bandScanning(), true);
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::bandWidth()
{
    for (int i = -512; i < 512; i++)
    {
        this->mOptions->setBandWidth(i);
        if (i > 0)
        {
            QCOMPARE(this->mOptions->bandWidth(), i);
        }
        else
        {
            QCOMPARE(this->mOptions->bandWidth(), 1);
        }
    }
}
//-----------------------------------------------------------------------------
void TestPrepareOptions::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
