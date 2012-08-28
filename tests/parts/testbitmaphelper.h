#ifndef BITMAPHELPERTEST_H
#define BITMAPHELPERTEST_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
class TestBitmapHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestBitmapHelper(QObject *parent = 0);

private:
    void compareSizeAndPixels(const QImage *image1, const QImage *image2) const;

private slots:
    void initTestCase();
    void rotate90();
    void rotate180();
    void rotate270();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // BITMAPHELPERTEST_H
