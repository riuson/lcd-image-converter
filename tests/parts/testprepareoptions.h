#ifndef TESTPREPAREOPTIONS_H
#define TESTPREPAREOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
//-----------------------------------------------------------------------------
class PrepareOptions;
//-----------------------------------------------------------------------------
class TestPrepareOptions : public QObject
{
    Q_OBJECT
public:
    explicit TestPrepareOptions(QObject *parent = 0);

private:
    PrepareOptions *mOptions;

private slots:
    void initTestCase();

    void convType();
    void monoType();
    void edge();
    void rotate();
    void flipVertical();
    void flipHorizontal();
    void inverse();

    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTPREPAREOPTIONS_H
