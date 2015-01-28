#include "widgeteditor.h"
#include "ui_widgeteditor.h"
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
WidgetEditor::WidgetEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetEditor)
{
    ui->setupUi(this);
}
//-----------------------------------------------------------------------------
WidgetEditor::~WidgetEditor()
{
    delete ui;
}
//-----------------------------------------------------------------------------
const QImage *WidgetEditor::image() const
{
    return NULL;
}
//-----------------------------------------------------------------------------
void WidgetEditor::setImage(const QImage *_value)
{
    Q_UNUSED(_value)
}
//-----------------------------------------------------------------------------
const QColor WidgetEditor::foreColor() const
{
    return QColor();
}
//-----------------------------------------------------------------------------
const QColor WidgetEditor::backColor() const
{
    return QColor();
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
