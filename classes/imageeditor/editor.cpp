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
} // end of namespace
//-----------------------------------------------------------------------------
