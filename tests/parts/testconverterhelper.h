#ifndef TESTCONVERTERHELPER_H
#define TESTCONVERTERHELPER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
#include "conversion_options.h"
#include "convimage.h"
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
            bool splitToRows, QVector<quint32> *packed, int *widthOut, int *heightOut);
    void prepareStringData(
            QVector<quint32> *source, int width, int height,
            bool splitToRows, DataBlockSize size, QString *string);

private slots:
    void initTestCase();
    void processPixels();
    void packData();
    void dataToString();
    void jsengineSetProperty();
    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
class TestConvImage : public ConvImage
{
    Q_OBJECT
public:
    enum Condition
    {
        CanBeDeleted,
        CannotBedDeleted
    };

    explicit TestConvImage(const QImage *image, QObject *parent = 0);
    virtual ~TestConvImage();

    void setCondition(Condition value);

private:
    Condition mCondition;
};

//-----------------------------------------------------------------------------
#endif // TESTCONVERTERHELPER_H
