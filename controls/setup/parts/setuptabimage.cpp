#include "setuptabimage.h"
#include "ui_setuptabimage.h"
//-----------------------------------------------------------------------------
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
SetupTabImage::SetupTabImage(Preset *preset, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabImage)
{
    ui->setupUi(this);
    this->mPreset = preset;

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
const QString &SetupTabImage::title()
{
    static const QString result = QObject::tr("Image");
    return result;
}
//-----------------------------------------------------------------------------
void SetupTabImage::matrixChanged()
{
    if (this->mPreset->image()->bytesOrder() == BytesOrderLittleEndian)
        this->ui->radioButtonLittleEndian->setChecked(true);
    else
        this->ui->radioButtonBigEndian->setChecked(true);

    int index = this->ui->comboBoxBlockSize->findData(this->mPreset->image()->blockSize());
    if (index >= 0)
        this->ui->comboBoxBlockSize->setCurrentIndex(index);

    if (this->mPreset->image()->bytesOrder() == BytesOrderLittleEndian)
        this->ui->radioButtonLittleEndian->setChecked(true);
    else
        this->ui->radioButtonBigEndian->setChecked(true);
}
//-----------------------------------------------------------------------------
void SetupTabImage::on_radioButtonLittleEndian_toggled(bool value)
{
    if (value)
        this->mPreset->image()->setBytesOrder(BytesOrderLittleEndian);
    else
        this->mPreset->image()->setBytesOrder(BytesOrderBigEndian);
}
//-----------------------------------------------------------------------------
void SetupTabImage::on_comboBoxBlockSize_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxBlockSize->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mPreset->image()->setBlockSize((DataBlockSize)a);
    }
}
//-----------------------------------------------------------------------------
