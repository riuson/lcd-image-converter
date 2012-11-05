#ifndef TESTTEMPLATEOPTIONS_H
#define TESTTEMPLATEOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QtTest/QtTest>
//-----------------------------------------------------------------------------
class TemplateOptions;
//-----------------------------------------------------------------------------
class TestTemplateOptions : public QObject
{
    Q_OBJECT
public:
    explicit TestTemplateOptions(QObject *parent = 0);

private:
    TemplateOptions *mOptions;

private slots:
    void initTestCase();

    void image();
    void font();

    void cleanupTestCase();
};
//-----------------------------------------------------------------------------
#endif // TESTTEMPLATEOPTIONS_H
