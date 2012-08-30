#ifndef TESTCONVERTERHELPER_H
#define TESTCONVERTERHELPER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include "conversionmatrix.h"
//-----------------------------------------------------------------------------
class TestConverterHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestConverterHelper(QObject *parent = 0);

private:
    ConversionMatrix *mMatrix;

private slots:
    void initTestCase();
    void processPixels();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTCONVERTERHELPER_H
