#include <QtTest/QtTest>

#include "testbitmaphelper.h"
#include "testbitstream.h"
#include "testconverterhelper.h"
#include "testfontoptions.h"
#include "testimageoptions.h"
#include "testmatrixoptions.h"
#include "testprepareoptions.h"
#include "testpresets.h"
#include "testreorderingoptions.h"
#include "testrle.h"
#include "testsettings.h"
#include "testtemplateoptions.h"

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

  TestSettings ts;
  QTest::qExec(&ts);

  return 0;
}
