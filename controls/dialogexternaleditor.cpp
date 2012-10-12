#include "dialogexternaleditor.h"
#include "ui_dialogexternaleditor.h"

#include <QFileDialog>
#include <QSettings>
//-----------------------------------------------------------------------------
DialogExternalEditor::DialogExternalEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExternalEditor)
{
    ui->setupUi(this);

    QSettings sett;
    sett.beginGroup("external-tools");
    QString imageEditor = sett.value("imageEditor", QVariant("gimp")).toString();
    this->ui->lineEdit->setText(imageEditor);
    sett.endGroup();
}
//-----------------------------------------------------------------------------
DialogExternalEditor::~DialogExternalEditor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogExternalEditor::done(int result)
{
    if (result == QDialog::Accepted)
    {
        QSettings sett;
        sett.beginGroup("external-tools");
        sett.setValue("imageEditor", QVariant(this->ui->lineEdit->text()));
        sett.endGroup();
    }
    QDialog::done(result);
}
//-----------------------------------------------------------------------------
void DialogExternalEditor::on_pushButtonBrowse_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.selectFile(this->ui->lineEdit->text());
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Select application"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEdit->setText(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
