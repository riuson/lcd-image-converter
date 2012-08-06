#include "dialogpreview.h"
#include "ui_dialogpresets.h"
//-----------------------------------------------------------------------------
#include "idatacontainer.h"
#include "converterhelper.h"
#include <QList>
//-----------------------------------------------------------------------------
DialogPreview::DialogPreview(IDataContainer *dataContainer, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPreview)
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
DialogPreview::~DialogPreview()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogPreview::on_comboBoxSampleKey_currentIndexChanged()
{
    this->updatePreview();
}
//-----------------------------------------------------------------------------
void DialogPreview::updatePreview()
{
    if (this->mData != NULL)
    {
        QString key = this->ui->comboBoxSampleKey->currentText();
        if (!key.isEmpty())
        {
            this->mImageOriginal = QImage(*this->mData->image(key));
            this->ui->labelOriginal->setPixmap(QPixmap::fromImage(this->mImageOriginal));
            //QImage processed = this->mConverter->preprocessImage(this->mImageOriginal);
            //this->ui->labelPreview->setPixmap(QPixmap::fromImage(processed));

            //BitmapData data;
            //this->mConverter->processImage(processed, &data);

            //QString str = this->mConverter->dataToString(data);
            //this->ui->plainTextEdit->setPlainText(str);

            QList<quint32> matrix;
            ConverterHelper::createMatrixColor(&matrix);

            QList<quint32> data;
            int width, height;
            ConverterHelper::pixelsData(&matrix, &this->mImageOriginal, &data, &width, &height);

            ConverterHelper::processPixels(&matrix, &data);

            QList<quint32> data2;
            int width2, height2;
            ConverterHelper::packData(&matrix, &data, width, height, &data2, &width2, &height2);
        }
    }
}
//-----------------------------------------------------------------------------
