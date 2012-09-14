#include "setuptabtemplates.h"
#include "ui_setuptabtemplates.h"
//-----------------------------------------------------------------------------
#include <QFileDialog>
#include "preset.h"
#include "templateoptions.h"
//-----------------------------------------------------------------------------
SetupTabTemplates::SetupTabTemplates(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabTemplates)
{
    ui->setupUi(this);
    this->mPreset = preset;

    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabTemplates::~SetupTabTemplates()
{
    delete ui;
}
//-----------------------------------------------------------------------------
const QString &SetupTabTemplates::title()
{
    static const QString result = QObject::tr("Templates");
    return result;
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::matrixChanged()
{
    this->ui->lineEditImage->setText(this->mPreset->templates()->image());
    this->ui->lineEditFont->setText(this->mPreset->templates()->font());
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::on_pushButtonBrowseImage_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open image template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mPreset->templates()->setImage(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
void SetupTabTemplates::on_pushButtonBrowseFont_clicked()
{
    QFileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setFilter(tr("Any file (*.*)"));
    dialog.setWindowTitle(tr("Open font template file"));

    if (dialog.exec() == QDialog::Accepted)
    {
        this->mPreset->templates()->setFont(dialog.selectedFiles().at(0));
    }
}
//-----------------------------------------------------------------------------
