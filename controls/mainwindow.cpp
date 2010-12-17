#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "widgetbitmapeditor.h"
#include "bitmapcontainer.h"
//-----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QImage im(":/images/winter");
    //QImage im2 = im.scaled(im.width() * 4, im.height() * 4, Qt::KeepAspectRatio, Qt::FastTransformation);
    //QPixmap pix = QPixmap::fromImage(im2);
    //this->ui->label->setPixmap(pix);
    WidgetBitmapEditor *w = new WidgetBitmapEditor(this->ui->tab);
    BitmapContainer *b = new BitmapContainer(this);
    w->assignImage(b->image());
}
//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
