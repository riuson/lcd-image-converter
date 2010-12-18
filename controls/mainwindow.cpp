#include "mainwindow.h"
#include "ui_mainwindow.h"
//-----------------------------------------------------------------------------
#include <QList>
#include <QFileDialog>

#include "editortabimage.h"
#include "bitmapcontainer.h"
#include "dialogsavechanges.h"
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
    bool cancel = false;
    if (doc->changed())
    {
        DialogSaveChanges dialog(this);
        dialog.exec();
        switch (dialog.answer())
        {
        case DialogSaveChanges::Save:
            doc->save(doc->fileName());
            break;
        case DialogSaveChanges::SaveAs:
            doc->save(doc->fileName());
            break;
        case DialogSaveChanges::DontSave:
            break;
        case DialogSaveChanges::Cancel:
            cancel = true;
            break;
        }
    }
    if (!cancel)
    {
        this->ui->tabWidget->removeTab(index);
        delete w;
    }
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
void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("XML Files (*.xml)"));
    dialog.setWindowTitle(tr("Open file"));
    if (dialog.exec() == QDialog::Accepted)
    {
    }
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);
    if (QFile::exists(doc->fileName()))
        doc->save(doc->fileName());
    else
        this->on_actionSave_As_triggered();
}
//-----------------------------------------------------------------------------
void MainWindow::on_actionSave_As_triggered()
{
    QWidget *w = this->ui->tabWidget->currentWidget();
    IDocument *doc = dynamic_cast<IDocument *> (w);

    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setFilter(tr("XML Files (*.xml)"));
    dialog.setWindowTitle(tr("Save file as"));
    if (dialog.exec() == QDialog::Accepted)
    {
        QString filename = dialog.selectedFiles().at(0);
        doc->save(filename);
    }
}
//-----------------------------------------------------------------------------
