#include "setuptabimage.h"
#include "ui_setuptabimage.h"

SetupTabImage::SetupTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetupTabImage)
{
    ui->setupUi(this);
}

SetupTabImage::~SetupTabImage()
{
    delete ui;
}
