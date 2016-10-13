#ifndef TESTBITMAPHELPER_H
#define TESTBITMAPHELPER_H

#include <QObject>
#include <QtTest/QtTest>
#include "bitmaphelper.h"

class TestBitmapHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestBitmapHelper(QObject *parent = 0);

private:
    void compareSizeAndPixels(const QImage *actual, const QImage *expected) const;

private slots:
    void initTestCase();
    void rotate90();
    void rotate180();
    void rotate270();
    void shiftUp();
    void shiftRight();
    void shiftDown();
    void shiftLeft();
    void flipHorizontal();
    void flipVertical();
    void cleanupTestCase();
};

#endif // TESTBITMAPHELPER_H
