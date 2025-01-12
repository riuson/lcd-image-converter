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

#include <QDomDocument>
#include <QSettings>
#include <QtXml>

namespace Settings
{
namespace Presets
{

const QString TemplateOptions::GroupName = QString("templates");
const QString TemplateOptions::FieldImages = QString("images");
const QString TemplateOptions::FieldFonts = QString("fonts");

TemplateOptions::TemplateOptions(QObject* parent) : QObject(parent)
{
  this->mImageTemplate = ":/templates/image_convert";
  this->mFontTemplate = ":/templates/font_convert";
}

const QString& TemplateOptions::image() const { return this->mImageTemplate; }

const QString& TemplateOptions::font() const { return this->mFontTemplate; }

void TemplateOptions::setImage(const QString& filename)
{
  if (this->mImageTemplate != filename) {
    this->mImageTemplate = filename;
    emit this->changed();
  }
}

void TemplateOptions::setFont(const QString& filename)
{
  if (this->mFontTemplate != filename) {
    this->mFontTemplate = filename;
    emit this->changed();
  }
}

bool TemplateOptions::load(QSettings* settings)
{
  bool result = false;

  settings->beginGroup(TemplateOptions::GroupName);

  QString sTemplateImage =
      settings->value(TemplateOptions::FieldImages, QString(":/templates/image_convert")).toString();
  QString sTemplateFont = settings->value(TemplateOptions::FieldFonts, QString(":/templates/font_convert")).toString();

  this->setImage(sTemplateImage);
  this->setFont(sTemplateFont);

  result = true;

  settings->endGroup();

  return result;
}

bool TemplateOptions::loadXmlElement(QDomElement element)
{
  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == TemplateOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return false;
  }

  QString sTemplateImage = QString(":/templates/image_convert");
  QString sTemplateFont = QString(":/templates/font_convert");

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == TemplateOptions::FieldImages) {
        sTemplateImage = e.text();
      }

      if (e.tagName() == TemplateOptions::FieldFonts) {
        sTemplateFont = e.text();
      }
    }

    nodeValue = nodeValue.nextSibling();
  }

  this->setImage(sTemplateImage);
  this->setFont(sTemplateFont);

  return true;
}

void TemplateOptions::save(QSettings* settings)
{
  settings->beginGroup(TemplateOptions::GroupName);

  settings->setValue(TemplateOptions::FieldImages, this->image());
  settings->setValue(TemplateOptions::FieldFonts, this->font());

  settings->endGroup();
}

void TemplateOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodeTemplate = element.ownerDocument().createElement(TemplateOptions::GroupName);
  element.appendChild(nodeTemplate);

  QDomElement nodeImages = element.ownerDocument().createElement(TemplateOptions::FieldImages);
  nodeTemplate.appendChild(nodeImages);
  nodeImages.appendChild(element.ownerDocument().createTextNode(this->image()));

  QDomElement nodeFonts = element.ownerDocument().createElement(TemplateOptions::FieldFonts);
  nodeTemplate.appendChild(nodeFonts);
  nodeFonts.appendChild(element.ownerDocument().createTextNode(this->font()));
}

QString TemplateOptions::groupName() const { return TemplateOptions::GroupName; }

} // namespace Presets
} // namespace Settings
