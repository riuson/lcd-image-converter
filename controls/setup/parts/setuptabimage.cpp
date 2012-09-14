#include "setuptabimage.h"
#include "ui_setuptabimage.h"
//-----------------------------------------------------------------------------
#include "conversionmatrix.h"
//-----------------------------------------------------------------------------
SetupTabImage::SetupTabImage(ConversionMatrix *matrix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabImage)
{
    ui->setupUi(this);
    this->mMatrix = matrix;

    this->ui->comboBoxBlockSize->addItem(tr("8 bit"), Data8);
    this->ui->comboBoxBlockSize->addItem(tr("16 bit"), Data16);
    this->ui->comboBoxBlockSize->addItem(tr("24 bit"), Data24);
    this->ui->comboBoxBlockSize->addItem(tr("32 bit"), Data32);

    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabImage::~SetupTabImage()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void SetupTabImage::matrixChanged()
{
    if (this->mMatrix->options()->bytesOrder() == BytesOrderLittleEndian)
        this->ui->radioButtonLittleEndian->setChecked(true);
    else
        this->ui->radioButtonBigEndian->setChecked(true);

    int index = this->ui->comboBoxBlockSize->findData(this->mMatrix->options()->blockSize());
    if (index >= 0)
        this->ui->comboBoxBlockSize->setCurrentIndex(index);

    if (this->mMatrix->options()->bytesOrder() == BytesOrderLittleEndian)
        this->ui->radioButtonLittleEndian->setChecked(true);
    else
        this->ui->radioButtonBigEndian->setChecked(true);
}
//-----------------------------------------------------------------------------
void SetupTabImage::on_radioButtonLittleEndian_toggled(bool value)
{
    if (value)
        this->mMatrix->options()->setBytesOrder(BytesOrderLittleEndian);
    else
        this->mMatrix->options()->setBytesOrder(BytesOrderBigEndian);
}
//-----------------------------------------------------------------------------
void SetupTabImage::on_comboBoxBlockSize_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxBlockSize->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setBlockSize((DataBlockSize)a);
    }
}
//-----------------------------------------------------------------------------
