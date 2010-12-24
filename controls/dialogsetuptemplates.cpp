#include "dialogsetuptemplates.h"
#include "ui_dialogsetuptemplates.h"

#include <QFileDialog>
#include <QSettings>
//-----------------------------------------------------------------------------
DialogSetupTemplates::DialogSetupTemplates(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetupTemplates)
{
    ui->setupUi(this);

    QSettings sett;
    sett.beginGroup("setup");

    this->ui->lineEditImage->setText(sett.value("templateImage", ":/templates/image_convert").toString());
    this->ui->lineEditFont->setText(sett.value("templateFont", ":/templates/font_convert").toString());

    sett.endGroup();
}
//-----------------------------------------------------------------------------
DialogSetupTemplates::~DialogSetupTemplates()
{
    QSettings sett;
    sett.beginGroup("setup");

    sett.setValue("templateImage", this->ui->lineEditImage->text());
    sett.setValue("templateFont", this->ui->lineEditFont->text());

    sett.endGroup();

    delete ui;
}
//-----------------------------------------------------------------------------
void DialogSetupTemplates::on_pushButtonBrowseImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open image template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEditImage->setText(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
void DialogSetupTemplates::on_pushButtonBrowseFont_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open font template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->ui->lineEditFont->setText(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
