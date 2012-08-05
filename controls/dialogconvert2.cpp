#include "dialogconvert2.h"
#include "ui_dialogconvert2.h"
//-----------------------------------------------------------------------------
DialogConvert2::DialogConvert2(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert2)
{
    ui->setupUi(this);
}
//-----------------------------------------------------------------------------
DialogConvert2::~DialogConvert2()
{
    delete ui;
}
//-----------------------------------------------------------------------------
