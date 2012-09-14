#include "templateoptions.h"
//-----------------------------------------------------------------------------
TemplateOptions::TemplateOptions(QObject *parent) :
    QObject(parent)
{
    this->mImageTemplate = ":/templates/image_convert";
    this->mFontTemplate = ":/templates/font_convert";
}
//-----------------------------------------------------------------------------
const QString &TemplateOptions::image() const
{
    return this->mImageTemplate;
}
//-----------------------------------------------------------------------------
const QString &TemplateOptions::font() const
{
    return this->mFontTemplate;
}
//-----------------------------------------------------------------------------
void TemplateOptions::setImage(const QString &filename)
{
    this->mImageTemplate = filename;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void TemplateOptions::setFont(const QString &filename)
{
    this->mFontTemplate = filename;

    emit this->changed();
}
//-----------------------------------------------------------------------------
