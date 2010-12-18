#include "dialogsavechanges.h"
#include "ui_dialogsavechanges.h"
//-----------------------------------------------------------------------------
DialogSaveChanges::DialogSaveChanges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSaveChanges)
{
    ui->setupUi(this);
    this->mAnswer = Cancel;
}
//-----------------------------------------------------------------------------
DialogSaveChanges::~DialogSaveChanges()
{
    delete ui;
}
//-----------------------------------------------------------------------------
int DialogSaveChanges::answer()
{
    return this->mAnswer;
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonSave_clicked()
{
    this->mAnswer = Save;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonSaveAs_clicked()
{
    this->mAnswer = SaveAs;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonDontSave_clicked()
{
    this->mAnswer = DontSave;
    this->close();
}
//-----------------------------------------------------------------------------
void DialogSaveChanges::on_buttonCancel_clicked()
{
    this->mAnswer = Cancel;
    this->close();
}
//-----------------------------------------------------------------------------
