#ifndef TESTMATRIXOPTIONS_H
#define TESTMATRIXOPTIONS_H

#include <QObject>
#include <QtTest/QtTest>

class MatrixOptions;

class TestMatrixOptions : public QObject
{
  Q_OBJECT
public:
  explicit TestMatrixOptions(QObject *parent = 0);

private:
  MatrixOptions *mOptions;

private slots:
  void initTestCase();

  void mask();
  void operationsCount();
  void operation();
  void operationAdd();
  void operationRemove();
  void operationRemoveAll();
  void operationReplace();

  void cleanupTestCase();
};

#endif // TESTMATRIXOPTIONS_H
