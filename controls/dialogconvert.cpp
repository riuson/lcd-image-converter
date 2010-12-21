#include "dialogconvert.h"
#include "ui_dialogconvert.h"

#include "converter.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);

    this->mWidgetSetup = NULL;

    this->mConverter = new Converter(this);
    QStringList names = this->mConverter->names();
    for (int i = 0; i < names.count(); i++)
    {
        IConverter *conv = this->mConverter->conv(names.at(i));
        this->ui->comboBoxType->addItem(conv->displayName(), QVariant(conv->name()));
    }
    this->ui->comboBoxType->setCurrentIndex(0);
}
//-----------------------------------------------------------------------------
DialogConvert::~DialogConvert()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxType_currentIndexChanged()
{
    if (this->mWidgetSetup != NULL)
    {
        this->ui->verticalLayout->removeWidget(this->mWidgetSetup);
        delete this->mWidgetSetup;
    }
    QVariant data = this->ui->comboBoxType->itemData(this->ui->comboBoxType->currentIndex());
    this->mWidgetSetup = this->mConverter->widgetSetup(data.toString());
    this->ui->verticalLayout->insertWidget(2, this->mWidgetSetup);
}
//-----------------------------------------------------------------------------
