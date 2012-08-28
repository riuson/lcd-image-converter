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
    QImage original = QImage(":/rotate/0");
    QImage expected = QImage(":/rotate/90");
    QImage test = BitmapHelper::rotate90(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::rotate180()
{
    QImage original = QImage(":/rotate/0");
    QImage expected = QImage(":/rotate/180");
    QImage test = BitmapHelper::rotate180(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::rotate270()
{
    QImage original = QImage(":/rotate/0");
    QImage expected = QImage(":/rotate/270");
    QImage test = BitmapHelper::rotate270(&original);

    this->compareSizeAndPixels(&expected, &test);
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::cleanupTestCase()
{
}
//-----------------------------------------------------------------------------
void TestBitmapHelper::compareSizeAndPixels(const QImage *image1, const QImage *image2) const
{
    if (image1->size() != image2->size())
        QFAIL("Size not match");

    for (int x = 0; x < image1->width(); x++)
    {
        for (int y = 0; y < image1->height(); y++)
        {
            QRgb valueExpected = image1->pixel(x, y);
            QRgb valueTest = image2->pixel(x, y);
            if (valueExpected != valueTest)
                QFAIL("Pixel's color not match");
        }
    }
}
//-----------------------------------------------------------------------------
