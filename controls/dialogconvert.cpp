#include "dialogconvert.h"
#include "ui_dialogconvert.h"

#include "converter.h"
#include "idatacontainer.h"
#include "bitmapdata.h"
//-----------------------------------------------------------------------------
DialogConvert::DialogConvert(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConvert)
{
    ui->setupUi(this);

    this->mWidgetSetup = NULL;
    this->mData = dataContainer;

    this->ui->comboBoxDataLength->addItem("8 bit", QVariant(8));
    this->ui->comboBoxDataLength->addItem("16 bit", QVariant(16));
    this->ui->comboBoxDataLength->addItem("32 bit", QVariant(32));

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

    if (this->mData != NULL)
    {
        this->ui->comboBoxSampleKey->addItems(this->mData->keys());
        this->ui->comboBoxSampleKey->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBoxSampleKey->setEnabled(false);
    }

    this->ui->checkBoxMirrorBytes->setChecked(this->mConverter->mirror());
    this->ui->checkBoxPack->setChecked(this->mConverter->pack());
    this->ui->checkBoxSwapBytes->setChecked(this->mConverter->swapBytes());
    switch (this->mConverter->length())
    {
    case IConverter::Data8:
        this->ui->comboBoxDataLength->setCurrentIndex(0);
        break;
    case IConverter::Data16:
        this->ui->comboBoxDataLength->setCurrentIndex(1);
        break;
    case IConverter::Data32:
        this->ui->comboBoxDataLength->setCurrentIndex(2);
        break;
    }

    this->connect(this->ui->checkBoxMirrorBytes, SIGNAL(toggled(bool)), SIGNAL(mirrorBytesChanged(bool)));
    this->connect(this->ui->checkBoxPack, SIGNAL(toggled(bool)), SIGNAL(dataPackChanged(bool)));
    this->connect(this->ui->checkBoxSwapBytes, SIGNAL(toggled(bool)), SIGNAL(swapBytesChanged(bool)));
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

    this->ui->checkBoxMirrorBytes->setChecked(this->mConverter->mirror());
    this->ui->checkBoxPack->setChecked(this->mConverter->pack());
    this->ui->checkBoxSwapBytes->setChecked(this->mConverter->swapBytes());
    switch (this->mConverter->length())
    {
    case IConverter::Data8:
        this->ui->comboBoxDataLength->setCurrentIndex(0);
        break;
    case IConverter::Data16:
        this->ui->comboBoxDataLength->setCurrentIndex(1);
        break;
    case IConverter::Data32:
        this->ui->comboBoxDataLength->setCurrentIndex(2);
        break;
    }

    this->mWidgetSetup = this->mConverter->widgetSetup();
    this->ui->splitterVertical->insertWidget(1, this->mWidgetSetup);
    this->mWidgetSetup->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->connect(this->mWidgetSetup, SIGNAL(settingsChanged()), SLOT(updatePreview()));
    this->connect(this, SIGNAL(dataLengthChanged(int)), this->mWidgetSetup, SLOT(dataLengthChanged(int)));
    this->connect(this, SIGNAL(dataPackChanged(bool)), this->mWidgetSetup, SLOT(dataPackChanged(bool)));
    this->connect(this, SIGNAL(mirrorBytesChanged(bool)), this->mWidgetSetup, SLOT(mirrorBytesChanged(bool)));
    this->connect(this, SIGNAL(swapBytesChanged(bool)), this->mWidgetSetup, SLOT(swapBytesChanged(bool)));

    //if (this->mConverter->conv().)

    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogConvert::on_comboBoxDataLength_currentIndexChanged()
{
    QVariant data = this->ui->comboBoxDataLength->itemData(this->ui->comboBoxDataLength->currentIndex());
    bool ok;
    int len = data.toInt(&ok);
    emit this->dataLengthChanged(len);
}
//-----------------------------------------------------------------------------
void DialogConvert::updatePreview()
{
    if (this->mData != NULL)
    {
        QString key = this->ui->comboBoxSampleKey->currentText();
        if (!key.isEmpty())
        {
            this->mImageOriginal = QImage(*this->mData->image(key));
            this->ui->labelOriginal->setPixmap(QPixmap::fromImage(this->mImageOriginal));
            QImage processed = this->mConverter->preprocessImage(this->mImageOriginal);
            this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));

            BitmapData data;
            this->mConverter->processImage(processed, &data);

            QString str = this->mConverter->dataToString(data);
            this->ui->plainTextEdit->setPlainText(str);
        }
    }
}
