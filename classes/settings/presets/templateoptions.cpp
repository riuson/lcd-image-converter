/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "templateoptions.h"
//-----------------------------------------------------------------------------
#include <QSettings>
//-----------------------------------------------------------------------------
const QString TemplateOptions::GroupName = QString("templates");
const QString TemplateOptions::FieldImages = QString("images");
const QString TemplateOptions::FieldFonts = QString("fonts");
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
bool TemplateOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        QString sTemplateImage = settings->value("imageTemplate", QString(":/templates/image_convert")).toString();
        QString sTemplateFont = settings->value("fontTemplate", QString(":/templates/font_convert")).toString();

        this->setImage(sTemplateImage);
        this->setFont(sTemplateFont);

        result = true;
    }
    else if (version == 2)
    {
        settings->beginGroup(TemplateOptions::GroupName);

        QString sTemplateImage = settings->value(TemplateOptions::FieldImages, QString(":/templates/image_convert")).toString();
        QString sTemplateFont = settings->value(TemplateOptions::FieldFonts, QString(":/templates/font_convert")).toString();

        this->setImage(sTemplateImage);
        this->setFont(sTemplateFont);

        result = true;

        settings->endGroup();
    }

    return result;
}
//-----------------------------------------------------------------------------
void TemplateOptions::save(QSettings *settings)
{
    settings->beginGroup(TemplateOptions::GroupName);

    settings->setValue(TemplateOptions::FieldImages, this->image());
    settings->setValue(TemplateOptions::FieldFonts, this->font());

    settings->endGroup();
}
//-----------------------------------------------------------------------------
