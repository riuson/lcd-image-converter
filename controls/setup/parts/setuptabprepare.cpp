#include "setuptabprepare.h"
#include "ui_setuptabprepare.h"

SetupTabPrepare::SetupTabPrepare(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabPrepare)
{
    ui->setupUi(this);
}

SetupTabPrepare::~SetupTabPrepare()
{
    delete ui;
}
