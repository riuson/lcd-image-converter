#include <QtTest/QtTest>
#include "testbitmaphelper.h"
#include "testconversionmatrixoptions.h"
#include "testconversionmatrix.h"
#include "testconverterhelper.h"
#include "testbitstream.h"
#include "testprepareoptions.h"
#include "testmatrixoptions.h"
#include "testimageoptions.h"
#include "testpresets.h"
//-----------------------------------------------------------------------------
int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    //TestConversionMatrixOptions tcmo;
    //QTest::qExec(&tcmo);

    //TestConversionMatrix tcm;
    //QTest::qExec(&tcm);

    TestConverterHelper tch;
    QTest::qExec(&tch);

    TestBitStream tbs;
    QTest::qExec(&tbs);

    TestPrepareOptions tpo;
    QTest::qExec(&tpo);

    TestMatrixOptions tmo;
    QTest::qExec(&tmo);

    TestImageOptions tio;
    QTest::qExec(&tio);

    TestPresets tp;
    QTest::qExec(&tp);

    return 0;
}
//-----------------------------------------------------------------------------
