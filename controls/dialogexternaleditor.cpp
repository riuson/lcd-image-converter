#include "dialogexternaleditor.h"
#include "ui_dialogexternaleditor.h"

#include <QFileDialog>
#include "externaltooloptions.h"
//-----------------------------------------------------------------------------
DialogExternalEditor::DialogExternalEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExternalEditor)
{
    ui->setupUi(this);

    this->ui->lineEdit->setText(ExternalToolOptions::imageEditor());
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
        ExternalToolOptions::setImageEditor(this->ui->lineEdit->text());
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
