#include <QtTest/QtTest>
#include "testbitmaphelper.h"
//-----------------------------------------------------------------------------
int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    return 0;
}
//-----------------------------------------------------------------------------
