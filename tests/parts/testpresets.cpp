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
    source.image()->setBytesOrder(BytesOrderBigEndian);
    source.prepare()->setFlipVertical(true);
    source.prepare()->setRotate(Rotate270);

    this->mOptions->matrix()->operationsRemoveAll();
    this->mOptions->initColor(5, 6, 5);
    this->mOptions->image()->setBytesOrder(BytesOrderBigEndian);
    this->mOptions->prepare()->setFlipVertical(true);
    this->mOptions->prepare()->setRotate(Rotate270);

    for (int i = 0; i < 10; i++)
    {
        this->mOptions->matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
        source.matrix()->operationAdd(quint32(0x12345678) >> 1, i + 1, (i % 2) == 0);
    }

    const QString name = "testpresets";
    if (!this->mOptions->save(name))
        QFAIL("preset not saved");

    this->mOptions->matrix()->operationsRemoveAll();
    this->mOptions->initMono(MonochromeTypeEdge, 1);

    if (!this->mOptions->load(name))
        QFAIL("preset not loaded");

    QCOMPARE(this->mOptions->prepare()->convType(),       source.prepare()->convType());
    QCOMPARE(this->mOptions->prepare()->monoType(),       source.prepare()->monoType());
    QCOMPARE(this->mOptions->prepare()->edge(),           source.prepare()->edge());
    QCOMPARE(this->mOptions->prepare()->rotate(),         source.prepare()->rotate());
    QCOMPARE(this->mOptions->prepare()->flipVertical(),   source.prepare()->flipVertical());
    QCOMPARE(this->mOptions->prepare()->flipHorizontal(), source.prepare()->flipHorizontal());
    QCOMPARE(this->mOptions->prepare()->inverse(),        source.prepare()->inverse());

    QCOMPARE(this->mOptions->matrix()->maskUsed(),       source.matrix()->maskUsed());
    QCOMPARE(this->mOptions->matrix()->maskAnd(),        source.matrix()->maskAnd());
    QCOMPARE(this->mOptions->matrix()->maskOr(),         source.matrix()->maskOr());
    QCOMPARE(this->mOptions->matrix()->maskFill(),       source.matrix()->maskFill());

    QCOMPARE(this->mOptions->image()->bytesOrder(),     source.image()->bytesOrder());
    QCOMPARE(this->mOptions->image()->blockSize(),      source.image()->blockSize());
}
//-----------------------------------------------------------------------------
void TestPresets::cleanupTestCase()
{
    delete this->mOptions;
}
//-----------------------------------------------------------------------------
