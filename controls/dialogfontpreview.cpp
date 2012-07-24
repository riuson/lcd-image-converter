#include "dialogfontpreview.h"
#include "ui_dialogfontpreview.h"
//-----------------------------------------------------------------------------
#include <QPainter>
#include "bitmaphelper.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
DialogFontPreview::DialogFontPreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontPreview)
{
    ui->setupUi(this);
    this->mOriginalImage = QImage(QImage(":/images/template").scaled(20, 10));

    this->on_spinBoxScale_valueChanged(this->ui->spinBoxScale->value());
}
//-----------------------------------------------------------------------------
DialogFontPreview::~DialogFontPreview()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void DialogFontPreview::setDocument(IDocument *document)
{
    this->mDocument = document;
}
//-----------------------------------------------------------------------------
void DialogFontPreview::on_lineEditText_textChanged(const QString &text)
{
    int width = 0, height = 0;
    QImage::Format format = QImage::Format_ARGB32;
    // find summary preview image size
    for (int i = 0; i < text.length(); i++)
    {
        QChar ch = text.at(i);
        if (this->mDocument->dataContainer()->keys().contains(ch))
        {
            QImage *image = this->mDocument->dataContainer()->image(ch);

            format = image->format();
            width += image->width();
            height = qMax(height, image->height());
        }
    }

    // create preview image
    QImage preview = QImage(width, height, format);
    QPixmap previewPixmap = QPixmap::fromImage(preview);
    QPainter painter(&previewPixmap);

    // place characters on image
    int x = 0;
    for (int i = 0; i < text.length(); i++)
    {
        QChar ch = text.at(i);
        if (this->mDocument->dataContainer()->keys().contains(ch))
        {
            QImage *image = this->mDocument->dataContainer()->image(ch);
            painter.drawImage(x, 0, *image);

            x += image->width();
        }
    }

    this->mOriginalImage = previewPixmap.toImage();

    // update preview
    this->on_spinBoxScale_valueChanged(this->ui->spinBoxScale->value());
}
//-----------------------------------------------------------------------------
void DialogFontPreview::on_spinBoxScale_valueChanged(int i)
{
    QImage scaled = BitmapHelper::createImageScaled(&this->mOriginalImage, i);
    this->mScaledPixmap = QPixmap::fromImage(scaled);
    this->ui->labelPreview->setPixmap(this->mScaledPixmap);
}
//-----------------------------------------------------------------------------
