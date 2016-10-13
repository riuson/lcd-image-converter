#ifndef TESTRLE_H
#define TESTRLE_H

#include <QObject>
#include <QtTest/QtTest>

template <class T> class QVector;

class TestRle : public QObject
{
    Q_OBJECT
public:
    explicit TestRle(QObject *parent = 0);

private:

private slots:
    void initTestCase();

    void compress2();
    void compress5();

    void cleanupTestCase();
};

#endif // TESTRLE_H
