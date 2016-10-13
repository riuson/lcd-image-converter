#include <QtTest/QtTest>
#include "testbitmaphelper.h"
#include "testconverterhelper.h"
#include "testbitstream.h"
#include "testprepareoptions.h"
#include "testmatrixoptions.h"
#include "testimageoptions.h"
#include "testreorderingoptions.h"
#include "testfontoptions.h"
#include "testtemplateoptions.h"
#include "testpresets.h"
#include "testrle.h"

int main(void)
{
    TestBitmapHelper tbh;
    QTest::qExec(&tbh);

    TestConverterHelper tch;
    QTest::qExec(&tch);

    TestBitStream tbs;
    QTest::qExec(&tbs);

    TestPrepareOptions tpo;
    QTest::qExec(&tpo);

    TestMatrixOptions tmo;
    QTest::qExec(&tmo);

    TestReorderingOptions tro;
    QTest::qExec(&tro);

    TestImageOptions tio;
    QTest::qExec(&tio);

    TestFontOptions tfo;
    QTest::qExec(&tfo);

    TestTemplateOptions tto;
    QTest::qExec(&tto);

    TestPresets tp;
    QTest::qExec(&tp);

    TestRle tr;
    QTest::qExec(&tr);

    return 0;
}

