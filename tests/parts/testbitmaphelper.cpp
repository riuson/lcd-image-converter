#include "testbitmaphelper.h"
//-----------------------------------------------------------------------------
TestBitmapHelper::TestBitmapHelper(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::initTestCase()
{
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::rotate90()
{
    QImage original = QImage("qrc:///rotate/0");
    QImage expected = QImage("qrc:///rotate/90");
    QImage test = BitmapHelper::rotate90(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::rotate180()
{
    QImage original = QImage("qrc:///rotate/0");
    QImage expected = QImage("qrc:///rotate/180");
    QImage test = BitmapHelper::rotate180(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::rotate270()
{
    QImage original = QImage("qrc:///rotate/0");
    QImage expected = QImage("qrc:///rotate/270");
    QImage test = BitmapHelper::rotate270(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::shiftUp()
{
    QImage original = QImage("qrc:///shift/original");
    QImage expected = QImage("qrc:///shift/up");
    QImage test = BitmapHelper::shiftUp(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::shiftRight()
{
    QImage original = QImage("qrc:///shift/original");
    QImage expected = QImage("qrc:///shift/right");
    QImage test = BitmapHelper::shiftRight(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::shiftDown()
{
    QImage original = QImage("qrc:///shift/original");
    QImage expected = QImage("qrc:///shift/down");
    QImage test = BitmapHelper::shiftDown(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::shiftLeft()
{
    QImage original = QImage("qrc:///shift/original");
    QImage expected = QImage("qrc:///shift/left");
    QImage test = BitmapHelper::shiftLeft(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::flipHorizontal()
{
    QImage original = QImage("qrc:///flip/0");
    QImage expected = QImage("qrc:///flip/horizontal");
    QImage test = BitmapHelper::flipHorizontal(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::flipVertical()
{
    QImage original = QImage("qrc:///flip/0");
    QImage expected = QImage("qrc:///flip/vertical");
    QImage test = BitmapHelper::flipVertical(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::cleanupTestCase()
{
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::compareSizeAndPixels(const QImage *actual, const QImage *expected) const
{
    QCOMPARE (actual->size(), expected->size());

    for (int x = 0; x < actual->width(); x++)
    {
        for (int y = 0; y < actual->height(); y++)
        {
            QRgb valueActual = actual->pixel(x, y);
            QRgb valueExpected = expected->pixel(x, y);
            QCOMPARE (valueActual, valueExpected);
        }
    }
}
//-----------------------------------------------------------------------------
