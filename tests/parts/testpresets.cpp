#include "testpresets.h"
//-----------------------------------------------------------------------------
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
TestPresets::TestPresets(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestPresets::initTestCase()
{
    this->mOptions = new Preset(this);
}
//-----------------------------------------------------------------------------
void TestPresets::loadSave()
{
    Preset source;
    source.initColor(5, 6, 5);
    source.prepare()->setScanMain(BottomToTop);
    source.prepare()->setScanSub(Backward);
    source.image()->setBytesOrder(BytesOrderBigEndian);
    source.image()->setBlockSize(Data16);
    source.image()->setSplitToRows(true);
    source.image()->setCompressionRle(true);

    this->mOptions->initColor(5, 6, 5);
    this->mOptions->prepare()->setScanMain(BottomToTop);
    this->mOptions->prepare()->setScanSub(Backward);
    this->mOptions->matrix()->operationsRemoveAll();
    this->mOptions->image()->setBytesOrder(BytesOrderBigEndian);
    this->mOptions->image()->setBlockSize(Data16);
    this->mOptions->image()->setSplitToRows(true);
    this->mOptions->image()->setCompressionRle(true);

    for (int i = 0; i < 10; i++)
    {
        this->mOptions->matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
        source.matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
    }

    const QString name = "testpresets";
    this->mOptions->save(name);

    this->mOptions->matrix()->operationsRemoveAll();
    this->mOptions->initMono(MonochromeTypeEdge, 1);

    if (!this->mOptions->load(name))
        QFAIL("preset not loaded");

    QCOMPARE(this->mOptions->prepare()->convType(), source.prepare()->convType());
    QCOMPARE(this->mOptions->prepare()->monoType(), source.prepare()->monoType());
    QCOMPARE(this->mOptions->prepare()->edge(),     source.prepare()->edge());
    QCOMPARE(this->mOptions->prepare()->scanMain(), source.prepare()->scanMain());
    QCOMPARE(this->mOptions->prepare()->scanSub(),  source.prepare()->scanSub());
    QCOMPARE(this->mOptions->prepare()->inverse(),  source.prepare()->inverse());

    QCOMPARE(this->mOptions->matrix()->maskUsed(),  source.matrix()->maskUsed());
    QCOMPARE(this->mOptions->matrix()->maskAnd(),   source.matrix()->maskAnd());
    QCOMPARE(this->mOptions->matrix()->maskOr(),    source.matrix()->maskOr());
    QCOMPARE(this->mOptions->matrix()->maskFill(),  source.matrix()->maskFill());

    QCOMPARE(this->mOptions->image()->bytesOrder(), source.image()->bytesOrder());
    QCOMPARE(this->mOptions->image()->blockSize(),  source.image()->blockSize());
    QCOMPARE(this->mOptions->image()->splitToRows(),    source.image()->splitToRows());
    QCOMPARE(this->mOptions->image()->compressionRle(), source.image()->compressionRle());
}
//-----------------------------------------------------------------------------
void TestPresets::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
