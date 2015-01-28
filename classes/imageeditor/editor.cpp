#include "editor.h"
#include "windoweditor.h"

#include <QColor>
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
Editor::Editor(QObject *parent) : QObject(parent)
{
    this->mWidget = new WindowEditor();
    this->connect(this->mWidget, SIGNAL(imageChanged()), SLOT(on_imageChanged()));
    this->connect(this->mWidget, SIGNAL(mouseMove(const QPoint *)), SLOT(on_mouseMove(const QPoint*)));
    this->connect(this->mWidget, SIGNAL(scaleSchanged(int)), SLOT(on_scaleSchanged(int)));
    this->mForeColor = new QColor();
    this->mBackColor = new QColor();
}
//-----------------------------------------------------------------------------
Editor::~Editor()
{
    delete this->mWidget;
    delete this->mForeColor;
    delete this->mBackColor;
}
//-----------------------------------------------------------------------------
QWidget *Editor::widget() const
{
    return this->mWidget;
}
//-----------------------------------------------------------------------------
const QImage *Editor::image() const
{
    return this->mWidget->image();
}
//-----------------------------------------------------------------------------
void Editor::setImage(const QImage *_value)
{
    this->mWidget->setImage(_value);
}
//-----------------------------------------------------------------------------
const QColor *Editor::foreColor() const
{
    return this->mForeColor;
}
//-----------------------------------------------------------------------------
const QColor *Editor::backColor() const
{
    return this->mBackColor;
}
//-----------------------------------------------------------------------------
int Editor::scale() const
{
    return this->mWidget->scale();
}
//-----------------------------------------------------------------------------
void Editor::on_imageChanged()
{
    emit this->imageChanged(this->image());
}
//-----------------------------------------------------------------------------
void Editor::on_mouseMove(const QPoint *point)
{
    emit this->mouseMoved(point);
}
//-----------------------------------------------------------------------------
void Editor::on_scaleSchanged(int scale)
{
    emit this->scaleChanged(scale);
}
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
