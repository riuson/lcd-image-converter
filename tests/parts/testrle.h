#ifndef TESTRLE_H
#define TESTRLE_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
//-----------------------------------------------------------------------------
template <class T> class QVector;
//-----------------------------------------------------------------------------
class TestRle : public QObject
{
    Q_OBJECT
public:
    explicit TestRle(QObject *parent = 0);

private:
    QVector<quint32> *mTestVector1;
    QVector<quint32> *mTestVector2;

private slots:
    void initTestCase();

    void compress();

    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTRLE_H
