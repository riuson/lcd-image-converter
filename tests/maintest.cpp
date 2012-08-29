#include <QtTest/QtTest>
#include "testbitmaphelper.h"
#include "testconversionmatrixoptions.h"
#include "testconversionmatrix.h"
//-----------------------------------------------------------------------------
int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    TestConversionMatrixOptions tcmo;
    QTest::qExec(&tcmo);

    TestConversionMatrix tcm;
    QTest::qExec(&tcm);

    return 0;
}
//-----------------------------------------------------------------------------
