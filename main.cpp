#include <QtGui/QApplication>
#include "mainwindow.h"
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("lcd-image-converter");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
//-----------------------------------------------------------------------------
