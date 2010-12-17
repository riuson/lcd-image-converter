#include "widgetbitmapeditor.h"
#include "ui_widgetbitmapeditor.h"

#define MIN_SCALE 1
#define MAX_SCALE 20
//-----------------------------------------------------------------------------
WidgetBitmapEditor::WidgetBitmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBitmapEditor)
{
    ui->setupUi(this);
    this->mImageOriginal = NULL;
    this->mScale = 10;
    this->updateState();
}
//-----------------------------------------------------------------------------
WidgetBitmapEditor::~WidgetBitmapEditor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::assignImage(QImage *image)
{
    this->ui->label->setPixmap(NULL);
    this->mImageOriginal = image;
    this->createImageScaled(this->mScale);
    this->updateState();
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::createImageScaled(quint32 scale)
{
    if (this->mImageOriginal != NULL)
    {
        quint32 width = this->mImageOriginal->width();
        quint32 height = this->mImageOriginal->height();
        //this->mImageScaled = new QImage(width * scale, height * scale, QImage::Format_RGB32);
        QImage scaled = this->mImageOriginal->scaled(width * scale, height * scale, Qt::KeepAspectRatio, Qt::FastTransformation);
        this->mPixmapScaled = QPixmap::fromImage(scaled);
        this->ui->label->setPixmap(this->mPixmapScaled);
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::updateState()
{
    this->ui->pushButtonMinus->setEnabled(this->mScale > MIN_SCALE);
    this->ui->pushButtonPlus->setEnabled(this->mScale < MAX_SCALE);
    this->ui->labelScale->setText(tr("Scale: %1").arg(this->mScale));
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonPlus_clicked()
{
    if (this->mScale < MAX_SCALE)
        this->mScale++;
    this->createImageScaled(this->mScale);
    this->updateState();
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonMinus_clicked()
{
    if (this->mScale > MIN_SCALE)
        this->mScale--;
    this->createImageScaled(this->mScale);
    this->updateState();
}
//-----------------------------------------------------------------------------
