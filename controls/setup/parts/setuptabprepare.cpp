#include "setuptabprepare.h"
#include "ui_setuptabprepare.h"
//-----------------------------------------------------------------------------
#include "conversionmatrix.h"
//-----------------------------------------------------------------------------
SetupTabPrepare::SetupTabPrepare(ConversionMatrix *matrix, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabPrepare)
{
    ui->setupUi(this);
    this->mMatrix = matrix;

    this->ui->comboBoxConversionType->addItem(tr("Monochrome"), ConversionTypeMonochrome);
    this->ui->comboBoxConversionType->addItem(tr("Grayscale"), ConversionTypeGrayscale);
    this->ui->comboBoxConversionType->addItem(tr("Color"), ConversionTypeColor);

    this->ui->comboBoxMonochromeType->addItem(tr("Edge"), MonochromeTypeEdge);
    this->ui->comboBoxMonochromeType->addItem(tr("Diffuse Dither"), MonochromeTypeDiffuseDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Ordered Dither"), MonochromeTypeOrderedDither);
    this->ui->comboBoxMonochromeType->addItem(tr("Threshold Dither"), MonochromeTypeThresholdDither);

    this->ui->comboBoxRotate->addItem(tr("None"), QVariant(RotateNone));
    this->ui->comboBoxRotate->addItem(tr("90 Clockwise"), QVariant(Rotate90));// \u00b0
    this->ui->comboBoxRotate->addItem(tr("180"), QVariant(Rotate180));
    this->ui->comboBoxRotate->addItem(tr("90 Counter-Clockwise"), QVariant(Rotate270));

    this->matrixChanged();
}
//-----------------------------------------------------------------------------
SetupTabPrepare::~SetupTabPrepare()
{
    delete ui;
}
//-----------------------------------------------------------------------------
const QString &SetupTabPrepare::title()
{
    static const QString result = QObject::tr("Prepare");
    return result;
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::matrixChanged()
{
    int index = this->ui->comboBoxConversionType->findData(this->mMatrix->options()->convType());
    if (index >= 0)
        this->ui->comboBoxConversionType->setCurrentIndex(index);

    index = this->ui->comboBoxMonochromeType->findData(this->mMatrix->options()->monoType());
    if (index >= 0)
        this->ui->comboBoxMonochromeType->setCurrentIndex(index);

    index = this->ui->comboBoxRotate->findData(this->mMatrix->options()->rotate());
    if (index >= 0)
        this->ui->comboBoxRotate->setCurrentIndex(index);

    this->ui->horizontalScrollBarEdge->setValue(this->mMatrix->options()->edge());

    this->ui->checkBoxFlipHorizontal->setChecked(this->mMatrix->options()->flipHorizontal());
    this->ui->checkBoxFlipVertical->setChecked(this->mMatrix->options()->flipVertical());
    this->ui->checkBoxInverse->setChecked(this->mMatrix->options()->inverse());
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxConversionType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxConversionType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setConvType((ConversionType)a);

        if (this->mMatrix->options()->convType() == ConversionTypeMonochrome)
        {
            this->ui->comboBoxMonochromeType->setEnabled(true);
            if (this->mMatrix->options()->monoType() == MonochromeTypeEdge)
                this->ui->horizontalScrollBarEdge->setEnabled(true);
            else
                this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
        else
        {
            this->ui->comboBoxMonochromeType->setEnabled(false);
            this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxMonochromeType_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxMonochromeType->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        this->mMatrix->options()->setMonoType((MonochromeType)a);

        if (this->mMatrix->options()->convType() == ConversionTypeMonochrome)
        {
            this->ui->comboBoxMonochromeType->setEnabled(true);
            if (this->mMatrix->options()->monoType() == MonochromeTypeEdge)
                this->ui->horizontalScrollBarEdge->setEnabled(true);
            else
                this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
        else
        {
            this->ui->comboBoxMonochromeType->setEnabled(false);
            this->ui->horizontalScrollBarEdge->setEnabled(false);
        }
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_comboBoxRotate_currentIndexChanged(int index)
{
    QVariant data = this->ui->comboBoxRotate->itemData(index);
    bool ok;
    int a = data.toInt(&ok);
    if (ok)
    {
        Rotate rotate = (Rotate)a;
        this->mMatrix->options()->setRotate(rotate);
    }
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxFlipHorizontal_toggled(bool value)
{
    this->mMatrix->options()->setFlipHorizontal(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxFlipVertical_toggled(bool value)
{
    this->mMatrix->options()->setFlipVertical(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_checkBoxInverse_toggled(bool value)
{
    this->mMatrix->options()->setInverse(value);
}
//-----------------------------------------------------------------------------
void SetupTabPrepare::on_horizontalScrollBarEdge_valueChanged(int value)
{
    this->mMatrix->options()->setEdge(value);
}
//-----------------------------------------------------------------------------
