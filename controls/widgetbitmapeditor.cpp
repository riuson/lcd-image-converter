#include "widgetbitmapeditor.h"
#include "ui_widgetbitmapeditor.h"
//-----------------------------------------------------------------------------
WidgetBitmapEditor::WidgetBitmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBitmapEditor)
{
    ui->setupUi(this);
    this->mImageOriginal = NULL;
    this->mScale = 10;
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
