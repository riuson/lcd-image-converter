#ifndef TESTCONVERTERHELPER_H
#define TESTCONVERTERHELPER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class Preset;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class TestConverterHelper : public QObject
{
    Q_OBJECT
public:
    explicit TestConverterHelper(QObject *parent = 0);

private:
    Preset *mPreset;
    void preparePackData(
            quint32 maskUsed, quint32 maskFill,
            QVector<quint32> *source, int width, int height,
            QVector<quint32> *packed, int *widthOut, int *heightOut);
    void prepareStringData(
            QVector<quint32> *source, int width, int height,
            DataBlockSize size, QString *string);

private slots:
    void initTestCase();
    void processPixels();
    void packData();
    void dataToString();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTCONVERTERHELPER_H
