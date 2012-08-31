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
    void preparePackData(
            quint32 maskUsed, quint32 maskFill,
            QVector<quint32> *source, int width, int height,
            QVector<quint32> *packed, int *widthOut, int *heightOut);

private slots:
    void initTestCase();
    void processPixels();
    void packData();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTCONVERTERHELPER_H
