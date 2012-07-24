#include "dialogfontpreview.h"
#include "ui_dialogfontpreview.h"
//-----------------------------------------------------------------------------
DialogFontPreview::DialogFontPreview(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontPreview)
{
    ui->setupUi(this);
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
}
//-----------------------------------------------------------------------------
void DialogFontPreview::on_spinBoxScale_valueChanged(int i)
{
}
//-----------------------------------------------------------------------------
