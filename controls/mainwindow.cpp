#include "mainwindow.h"
#include "ui_mainwindow.h"
//-----------------------------------------------------------------------------
#include <QList>

#include "editortabimage.h"
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
QString MainWindow::findAvailableName(const QString &prefix)
{
    QString result = prefix;
    QList<QString> names;
    for (int i = 0; i < this->ui->tabWidget->count(); i++)
    {
        QWidget *w = this->ui->tabWidget->widget(i);
        IDocument *doc = dynamic_cast<IDocument *> (w);
        names.append(doc->documentName());
    }
    int i = 1;
    while (true)
    {
        QString name = QString("%1 %2").arg(prefix).arg(i++);
        if (!names.contains(name))
        {
            result = name;
            break;
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QWidget *w = this->ui->tabWidget->widget(index);
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (doc->changed())
    {
        doc->save("");
    }
    this->ui->tabWidget->removeTab(index);
    delete w;
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionNew_Image_triggered()
{
    EditorTabImage *e = new EditorTabImage(this);
    QString name = e->documentName();
    name = this->findAvailableName(name);
    e->setDocumentName(name);
    this->ui->tabWidget->addTab(e, name);
}
//-----------------------------------------------------------------------------
