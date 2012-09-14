#include "setuptabmatrix.h"
#include "ui_setuptabmatrix.h"

SetupTabMatrix::SetupTabMatrix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabMatrix)
{
    ui->setupUi(this);
}

SetupTabMatrix::~SetupTabMatrix()
{
    delete ui;
}
