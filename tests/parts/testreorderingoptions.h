#ifndef TESTREORDERINGOPTIONS_H
#define TESTREORDERINGOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

class ReorderingOptions;

class TestReorderingOptions : public QObject
{
    Q_OBJECT
public:
    explicit TestReorderingOptions(QObject *parent = 0);

private:
    ReorderingOptions *mOptions;

private slots:
    void initTestCase();

    void operationsCount();
    void operation();
    void operationAdd();
    void operationRemove();
    void operationRemoveAll();
    void operationReplace();

    void cleanupTestCase();
};

#endif // TESTREORDERINGOPTIONS_H
