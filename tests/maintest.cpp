#include <QtTest/QtTest>
#include "testbitmaphelper.h"
#include "testconversionmatrixoptions.h"
//-----------------------------------------------------------------------------
int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    TestConversionMatrixOptions tcmo;
    QTest::qExec(&tcmo);

    return 0;
}
//-----------------------------------------------------------------------------
