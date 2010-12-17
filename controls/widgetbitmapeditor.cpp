#include "widgetbitmapeditor.h"
#include "ui_widgetbitmapeditor.h"

#include <QMouseEvent>
#include <QColorDialog>
#include <QPainter>
//-----------------------------------------------------------------------------
WidgetBitmapEditor::WidgetBitmapEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetBitmapEditor)
{
    ui->setupUi(this);
    this->mImageOriginal = NULL;
    this->mScale = 10;
    this->ui->spinBoxScale->setValue(this->mScale);
    this->ui->label->installEventFilter(this);

    this->mColor1 = QColor("black");
    this->mColor2 = QColor("white");

    this->mPixmapColor1 = QPixmap(16, 16);
    this->mPixmapColor2 = QPixmap(16, 16);
    this->mPixmapColor1.fill(this->mColor1);
    this->mPixmapColor2.fill(this->mColor2);
    this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
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
bool WidgetBitmapEditor::eventFilter(QObject *obj, QEvent *event)
{
    bool result = false;
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent *me = dynamic_cast<QMouseEvent *>(event);
        // get coordinates
        quint32 xscaled = me->pos().x();
        quint32 yscaled = me->pos().y();
        quint32 xreal = xscaled / this->mScale;
        quint32 yreal = yscaled / this->mScale;
        if(xreal < this->mImageOriginal->width() && yreal < this->mImageOriginal->height())
        {
            // show coordinates
            this->ui->labelCoordinates->setText(QString("%1,%2").arg(xreal).arg(yreal));
            // get buttons
            bool buttonLeft = (me->buttons() & Qt::LeftButton) == Qt::LeftButton;
            bool buttonRight = (me->buttons() & Qt::RightButton) == Qt::RightButton;
            // draw on pixmap
            if (buttonLeft || buttonRight)
            {
                QPainter painterScaled(&this->mPixmapScaled);
                QColor color;
                if (buttonLeft)
                    color = this->mColor1;
                if (buttonRight)
                    color = this->mColor2;

                if (this->mScale == 1)
                {
                    painterScaled.setPen(color);
                    painterScaled.drawPoint(xscaled, yscaled);
                }
                else
                {
                    painterScaled.fillRect(xreal * this->mScale,
                                           yreal * this->mScale,
                                           this->mScale,
                                           this->mScale,
                                           color);
                }

                this->ui->label->setPixmap(this->mPixmapScaled);

                QPainter painterOriginal(this->mImageOriginal);
                painterOriginal.setPen(color);
                painterOriginal.drawPoint(xreal, yreal);
            }
        }
        event->accept();
    }
    else
    {
        result = QObject::eventFilter(obj, event);
    }
    return result;
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
void WidgetBitmapEditor::on_spinBoxScale_valueChanged(int value)
{
    this->mScale = value;
    this->createImageScaled(this->mScale);
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonColor1_clicked()
{
    QColorDialog dialog(this->mColor1, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor1 = dialog.selectedColor();
        this->mPixmapColor1.fill(this->mColor1);
        this->ui->pushButtonColor1->setIcon(QIcon(this->mPixmapColor1));
    }
}
//-----------------------------------------------------------------------------
void WidgetBitmapEditor::on_pushButtonColor2_clicked()
{
    QColorDialog dialog(this->mColor2, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        this->mColor2 = dialog.selectedColor();
        this->mPixmapColor2.fill(this->mColor2);
        this->ui->pushButtonColor2->setIcon(QIcon(this->mPixmapColor2));
    }
}
//-----------------------------------------------------------------------------
