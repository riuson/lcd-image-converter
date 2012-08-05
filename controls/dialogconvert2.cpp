#include "dialogconvert2.h"
#include "ui_dialogconvert2.h"
//-----------------------------------------------------------------------------
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
DialogConvert2::DialogConvert2(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert2)
{
    ui->setupUi(this);
    this->mData = dataContainer;

    if (this->mData != NULL)
    {
        this->ui->comboBoxSampleKey->addItems(this->mData->keys());
        this->ui->comboBoxSampleKey->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBoxSampleKey->setEnabled(false);
    }

}
//-----------------------------------------------------------------------------
DialogConvert2::~DialogConvert2()
{
    delete ui;
}
//-----------------------------------------------------------------------------
