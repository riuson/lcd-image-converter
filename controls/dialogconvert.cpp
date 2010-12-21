#include "dialogconvert.h"
#include "ui_dialogconvert.h"

#include "converter.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);

    this->mWidgetSetup = NULL;
    this->mData = dataContainer;

    this->mConverter = new Converter(this);
    QString name = this->mConverter->name();
    QStringList names = this->mConverter->names();
    for (int i = 0; i < names.count(); i++)
    {
        IConverter *conv = this->mConverter->conv(names.at(i));
        this->ui->comboBoxType->addItem(conv->displayName(), QVariant(conv->name()));
    }

    if (names.contains(name))
        this->ui->comboBoxType->setCurrentIndex(names.indexOf(name));
    else
        this->ui->comboBoxType->setCurrentIndex(0);

    this->ui->comboBoxSampleKey->addItems(this->mData->keys());
    this->ui->comboBoxSampleKey->setCurrentIndex(0);
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
    this->mConverter->selectConv(data.toString());
    this->mWidgetSetup = this->mConverter->widgetSetup();
    this->ui->verticalLayout->insertWidget(2, this->mWidgetSetup);
    this->connect(this->mWidgetSetup, SIGNAL(settingsChanged()), SLOT(updatePreview()));

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::updatePreview()
{
    QString key = this->ui->comboBoxSampleKey->currentText();
    if (!key.isEmpty())
    {
        this->mImageOriginal = QImage(*this->mData->image(key));
        this->ui->labelOriginal->setPixmap(QPixmap::fromImage(this->mImageOriginal));
        QImage processed = this->mConverter->preprocessImage(this->mImageOriginal);
        this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));
    }
}
