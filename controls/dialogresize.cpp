#include "dialogresize.h"
#include "ui_dialogresize.h"
//-----------------------------------------------------------------------------
DialogResize::DialogResize(int width, int height, int offsetX, int offsetY, bool center, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogResize)
{
    ui->setupUi(this);
    this->mWidth = width;
    this->mHeight = height;
    this->mOffsetX = offsetX;
    this->mOffsetY = offsetY;
    this->mCenter = center;

    this->ui->spinBoxWidth->setValue(width);
    this->ui->spinBoxHeight->setValue(height);
    this->ui->spinBoxOffsetX->setValue(offsetX);
    this->ui->spinBoxOffsetY->setValue(offsetY);
    this->ui->checkBoxCenter->setChecked(center);
}
//-----------------------------------------------------------------------------
DialogResize::~DialogResize()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogResize::getResizeInfo(int *width, int *height, int *offsetX, int *offsetY, bool *center)
{
    *width = this->mWidth;
    *height = this->mHeight;
    *offsetX = this->mOffsetX;
    *offsetY = this->mOffsetY;
    *center = this->mCenter;
}
//-----------------------------------------------------------------------------
void DialogResize::on_checkBoxCenter_stateChanged(int state)
{
    bool manualOffset = (state == Qt::Unchecked);
    this->ui->labelOffsetX->setEnabled(manualOffset);
    this->ui->labelOffsetY->setEnabled(manualOffset);
    this->ui->spinBoxOffsetX->setEnabled(manualOffset);
    this->ui->spinBoxOffsetY->setEnabled(manualOffset);
    this->mCenter = !manualOffset;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxWidth_valueChanged(int value)
{
    this->mWidth = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxHeight_valueChanged(int value)
{
    this->mHeight = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxOffsetX_valueChanged(int value)
{
    this->mOffsetX = value;
}
//-----------------------------------------------------------------------------
void DialogResize::on_spinBoxOffsetY_valueChanged(int value)
{
    this->mOffsetY = value;
}
//-----------------------------------------------------------------------------
