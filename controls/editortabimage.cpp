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

    this->mContainer = new BitmapContainer(this);
    this->mEditor->assignImage(this->mContainer->image());

    this->connect(this->mEditor, SIGNAL(dataChanged()), SLOT(onDataChanged()));

    this->mDocumentName = tr("Image", "new image name");
    this->mFileName = "";
    this->mDataChanged = false;
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
void EditorTabImage::onDataChanged()
{
    this->mDataChanged = true;
}
//-----------------------------------------------------------------------------
void EditorTabImage::load(const QString &fileName)
{
}
//-----------------------------------------------------------------------------
void EditorTabImage::save(const QString &fileName)
{
}
//-----------------------------------------------------------------------------
bool EditorTabImage::changed()
{
    return this->mDataChanged;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::fileName()
{
    return this->mFileName;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::documentName()
{
    return this->mDocumentName;
}
//-----------------------------------------------------------------------------
void EditorTabImage::setDocumentName(const QString &value)
{
    this->mDocumentName = value;
}
//-----------------------------------------------------------------------------
