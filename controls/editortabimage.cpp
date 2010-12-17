#include "editortabimage.h"
#include "ui_editortabimage.h"
//-----------------------------------------------------------------------------
#include <QHBoxLayout>

#include "widgetbitmapeditor.h"
#include "bitmapcontainer.h"
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mEditor = new WidgetBitmapEditor(this);
    layout->addWidget(this->mEditor);

    BitmapContainer *b = new BitmapContainer(this);
    this->mEditor->assignImage(b->image());
}
//-----------------------------------------------------------------------------
EditorTabImage::~EditorTabImage()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void EditorTabImage::changeEvent(QEvent *e)
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
