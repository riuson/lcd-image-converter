#include <QtTest/QtTest>
#include "testbitmaphelper.h"
#include "testconversionmatrixoptions.h"
#include "testconversionmatrix.h"
#include "testconverterhelper.h"
#include "testbitstream.h"
//-----------------------------------------------------------------------------
int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    TestConversionMatrixOptions tcmo;
    QTest::qExec(&tcmo);

    TestConversionMatrix tcm;
    QTest::qExec(&tcm);

    TestConverterHelper tch;
    QTest::qExec(&tch);

    TestBitStream tbs;
    QTest::qExec(&tbs);

    return 0;
}
//-----------------------------------------------------------------------------
