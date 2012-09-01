#ifndef TESTCONVERSIONMATRIXOPTIONS_H
#define TESTCONVERSIONMATRIXOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include <QVector>
#include "bitmaphelper.h"
#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
class TestConversionMatrixOptions : public QObject
{
    Q_OBJECT
public:
    explicit TestConversionMatrixOptions(QObject *parent = 0);

private:
    QVector<quint32> *mMatrixData;
    ConversionMatrixOptions *mOptions;

private slots:
    void initTestCase();

    void bytesOrder();
    void setBytesOrder();
    void convType();
    void setConvType();
    void monoType();
    void setMonoType();
    void edge();
    void setEdge();
    void blockSize();
    void setBlockSize();

    void rotate();
    void setRotate();
    void flipVertical();
    void setFlipVertical();
    void flipHorizontal();
    void setFlipHorizontal();
    void inverse();
    void setInverse();

    void mask();
    void setMask();

    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTCONVERSIONMATRIXOPTIONS_H
