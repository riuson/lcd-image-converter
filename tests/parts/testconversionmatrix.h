#ifndef TESTCONVERSIONMATRIX_H
#define TESTCONVERSIONMATRIX_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include "conversionmatrix.h"
//-----------------------------------------------------------------------------
class TestConversionMatrix : public QObject
{
    Q_OBJECT
public:
    explicit TestConversionMatrix(QObject *parent = 0);

private:
    ConversionMatrix *mMatrix;

private slots:
    void initTestCase();
    void operationsCount();
    void operation();
    void operationAdd();
    void operationRemove();
    void operationRemoveAll();
    void operationReplace();
    void loadSave();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTCONVERSIONMATRIX_H
