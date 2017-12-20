#include "testbitmaphelper.h"

TestBitmapHelper::TestBitmapHelper(QObject *parent) :
  QObject(parent)
{
}

void TestBitmapHelper::initTestCase()
{
}

void TestBitmapHelper::rotate90()
{
  QImage original = QImage(":/rotate/0");
  QImage expected = QImage(":/rotate/90");
  QImage test = Parsing::Conversion::BitmapHelper::rotate90(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::rotate180()
{
  QImage original = QImage(":/rotate/0");
  QImage expected = QImage(":/rotate/180");
  QImage test = Parsing::Conversion::BitmapHelper::rotate180(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::rotate270()
{
  QImage original = QImage(":/rotate/0");
  QImage expected = QImage(":/rotate/270");
  QImage test = Parsing::Conversion::BitmapHelper::rotate270(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::shiftUp()
{
  QImage original = QImage(":/shift/original");
  QImage expected = QImage(":/shift/up");
  QImage test = Parsing::Conversion::BitmapHelper::shiftUp(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::shiftRight()
{
  QImage original = QImage(":/shift/original");
  QImage expected = QImage(":/shift/right");
  QImage test = Parsing::Conversion::BitmapHelper::shiftRight(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::shiftDown()
{
  QImage original = QImage(":/shift/original");
  QImage expected = QImage(":/shift/down");
  QImage test = Parsing::Conversion::BitmapHelper::shiftDown(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::shiftLeft()
{
  QImage original = QImage(":/shift/original");
  QImage expected = QImage(":/shift/left");
  QImage test = Parsing::Conversion::BitmapHelper::shiftLeft(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::flipHorizontal()
{
  QImage original = QImage(":/flip/0");
  QImage expected = QImage(":/flip/horizontal");
  QImage test = Parsing::Conversion::BitmapHelper::flipHorizontal(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::flipVertical()
{
  QImage original = QImage(":/flip/0");
  QImage expected = QImage(":/flip/vertical");
  QImage test = Parsing::Conversion::BitmapHelper::flipVertical(&original);

  this->compareSizeAndPixels(&expected, &test);
}

void TestBitmapHelper::cleanupTestCase()
{
}

void TestBitmapHelper::compareSizeAndPixels(const QImage *actual, const QImage *expected) const
{
  QCOMPARE (actual->size(), expected->size());

  for (int x = 0; x < actual->width(); x++) {
    for (int y = 0; y < actual->height(); y++) {
      QRgb valueActual = actual->pixel(x, y);
      QRgb valueExpected = expected->pixel(x, y);
      QCOMPARE (valueActual, valueExpected);
    }
  }
}

