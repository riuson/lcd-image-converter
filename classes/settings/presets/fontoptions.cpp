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

#include "fontoptions.h"

#include <QStringList>
#include <QSettings>
#include <QTextCodec>
#include <QtXml>
#include <QDomDocument>

namespace Settings
{
namespace Presets
{

const QString FontOptions::GroupName = QString("font");
const QString FontOptions::FieldBom = QString("bom");
const QString FontOptions::FieldSortOrder = QString("sortOrder");
const QString FontOptions::FieldCodec = QString("codec");
const QString FontOptions::FieldSkipMissingCharacters = QString("skipMissingCharacters");
const QString FontOptions::FieldEscapedCharacters = QString("escapeChars");
const QString FontOptions::FieldEscapePrefix = QString("escapePrefix");
const QString FontOptions::FieldEscapeSuffix = QString("escapeSuffix");
const QString FontOptions::FieldCompactGlyphs = QString("compactGlyphs");

FontOptions::FontOptions(QObject *parent) :
  QObject(parent)
{
  this->mBom = false;
  this->mSortOrder = Parsing::Conversion::Options::CharactersSortOrder::Ascending;

  if (FontOptions::encodings().contains("UTF-8")) {
    this->mEncoding = "UTF-8";
  } else {
    this->mEncoding = FontOptions::encodings().at(0);
  }

  this->mSkipMissingCharacters = false;

  this->mEscapedCharacters = "@\"'";
  this->mEscapePrefix = "\\";
  this->mEscapeSuffix = QString();
  this->mCompactGlyphs = false;
}

bool FontOptions::bom() const
{
  return this->mBom;
}

const QString &FontOptions::encoding() const
{
  return this->mEncoding;
}

Parsing::Conversion::Options::CharactersSortOrder FontOptions::sortOrder() const
{
  return this->mSortOrder;
}

bool FontOptions::skipMissingCharacters() const
{
  return this->mSkipMissingCharacters;
}

const QString &FontOptions::escapedCharacters() const
{
  return this->mEscapedCharacters;
}

const QString &FontOptions::escapePrefix() const
{
  return this->mEscapePrefix;
}

const QString &FontOptions::escapeSuffix() const
{
  return this->mEscapeSuffix;
}

bool FontOptions::compactGlyphs() const
{
  return this->mCompactGlyphs;
}

void FontOptions::setBom(bool value)
{
  if (this->mBom != value) {
    this->mBom = value;
    emit this->changed();
  }
}

void FontOptions::setEncoding(const QString &value)
{
  if (FontOptions::encodings().contains(value)) {
    if (this->mEncoding != value) {
      this->mEncoding = value;
      emit this->changed();
    }
  }
}

void FontOptions::setSortOrder(Parsing::Conversion::Options::CharactersSortOrder value)
{
  if (this->mSortOrder != value) {
    this->mSortOrder = value;
    emit this->changed();
  }
}

void FontOptions::setSkipMissingCharacters(bool value)
{
  if (this->mSkipMissingCharacters != value) {
    this->mSkipMissingCharacters = value;
    emit this->changed();
  }
}

void FontOptions::setEscapedCharacters(const QString &value)
{
  if (this->mEscapedCharacters != value) {
    this->mEscapedCharacters = value;
    emit this->changed();
  }
}

void FontOptions::setEscapePrefix(const QString &value)
{
  if (this->mEscapePrefix != value) {
    this->mEscapePrefix = value;
    emit this->changed();
  }
}

void FontOptions::setEscapeSuffix(const QString &value)
{
  if (this->mEscapeSuffix != value) {
    this->mEscapeSuffix = value;
    emit this->changed();
  }
}

void FontOptions::setCompactGlyphs(bool value)
{
  if (this->mCompactGlyphs != value) {
    this->mCompactGlyphs = value;
    emit this->changed();
  }
}

bool FontOptions::load(QSettings *settings)
{
  bool result = false;

  settings->beginGroup(FontOptions::GroupName);

  quint32 uBom;
  quint32 uSortOrder;
  QString sEncoding;
  quint32 uSkipMissingCharacters;
  QString sEscapedCharacters;
  QString sEscapePrefix;
  QString sEscapeSuffix;
  quint32 uCompactGlyphs;

  uBom = settings->value(FontOptions::FieldBom, int(0)).toInt(&result);

  if (result) {
    uSortOrder = settings->value(FontOptions::FieldSortOrder, int(Parsing::Conversion::Options::CharactersSortOrder::None)).toInt(&result);
  }

  if (result) {
    sEncoding = settings->value(FontOptions::FieldCodec, QString("UTF-8")).toString();
  }

  if (result) {
    sEscapedCharacters = settings->value(FontOptions::FieldEscapedCharacters, QString("@\"'")).toString();
  }

  if (result) {
    uSkipMissingCharacters = settings->value(FontOptions::FieldSkipMissingCharacters, int(0)).toInt(&result);
  }

  if (result) {
    sEscapePrefix = settings->value(FontOptions::FieldEscapePrefix, QString("\\")).toString();
  }

  if (result) {
    sEscapeSuffix = settings->value(FontOptions::FieldEscapeSuffix, QString()).toString();
  }

  if (result) {
    uCompactGlyphs = settings->value(FontOptions::FieldCompactGlyphs, int(0)).toInt(&result);
  }

  if (result) {
    this->setBom((bool)uBom);
    this->setEncoding(sEncoding);
    this->setSortOrder((Parsing::Conversion::Options::CharactersSortOrder)uSortOrder);
    this->setSkipMissingCharacters((bool)uSkipMissingCharacters);
    this->setEscapedCharacters(sEscapedCharacters);
    this->setEscapePrefix(sEscapePrefix);
    this->setEscapeSuffix(sEscapeSuffix);
    this->setCompactGlyphs((bool)uCompactGlyphs);
  }

  settings->endGroup();

  return result;
}

bool FontOptions::loadXmlElement(QDomElement element)
{
  bool result = false;

  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == FontOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return result;
  }

  quint32 uBom = 0;
  quint32 uSortOrder = int(Parsing::Conversion::Options::CharactersSortOrder::None);
  QString sEncoding = "UTF-8";
  quint32 uSkipMissingCharacters = 0;
  QString sEscapedCharacters = "@\"'";
  QString sEscapePrefix = "\\";
  QString sEscapeSuffix = QString();
  quint32 uCompactGlyphs = 0;

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == FontOptions::FieldBom) {
        QString str = e.text();
        uBom = str.toUInt(&result);
      }

      if (e.tagName() == FontOptions::FieldSortOrder) {
        QString str = e.text();
        uSortOrder = str.toUInt(&result);
      }

      if (e.tagName() == FontOptions::FieldCodec) {
        sEncoding = e.text();
      }

      if (e.tagName() == FontOptions::FieldSkipMissingCharacters) {
        QString str = e.text();
        uSkipMissingCharacters = str.toUInt(&result);
      }

      if (e.tagName() == FontOptions::FieldEscapedCharacters) {
        sEscapedCharacters = e.text();
      }

      if (e.tagName() == FontOptions::FieldEscapePrefix) {
        sEscapePrefix = e.text();
      }

      if (e.tagName() == FontOptions::FieldEscapeSuffix) {
        sEscapeSuffix = e.text();
      }

      if (e.tagName() == FontOptions::FieldCompactGlyphs) {
        QString str = e.text();
        uCompactGlyphs = str.toUInt(&result);
      }

      if (!result) {
        break;
      }
    }

    nodeValue = nodeValue.nextSibling();
  }

  if (result) {
    this->setBom((bool)uBom);
    this->setEncoding(sEncoding);
    this->setSortOrder((Parsing::Conversion::Options::CharactersSortOrder)uSortOrder);
    this->setSkipMissingCharacters((bool)uSkipMissingCharacters);
    this->setEscapedCharacters(sEscapedCharacters);
    this->setEscapePrefix(sEscapePrefix);
    this->setEscapeSuffix(sEscapeSuffix);
    this->setCompactGlyphs((bool)uCompactGlyphs);
  }

  return result;
}

void FontOptions::save(QSettings *settings)
{
  settings->beginGroup(FontOptions::GroupName);

  settings->setValue(FontOptions::FieldBom, QString("%1").arg((int)this->bom()));
  settings->setValue(FontOptions::FieldSortOrder, QString("%1").arg((int)this->sortOrder()));
  settings->setValue(FontOptions::FieldCodec,  this->encoding());
  settings->setValue(FontOptions::FieldSkipMissingCharacters, QString("%1").arg((int)this->skipMissingCharacters()));
  settings->setValue(FontOptions::FieldEscapedCharacters,  this->escapedCharacters());
  settings->setValue(FontOptions::FieldEscapePrefix,  this->escapePrefix());
  settings->setValue(FontOptions::FieldEscapeSuffix,  this->escapeSuffix());
  settings->setValue(FontOptions::FieldCompactGlyphs, QString("%1").arg((int)this->compactGlyphs()));

  settings->endGroup();
}

void FontOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodeFont = element.ownerDocument().createElement(FontOptions::GroupName);
  element.appendChild(nodeFont);

  QDomElement nodeBom = element.ownerDocument().createElement(FontOptions::FieldBom);
  nodeFont.appendChild(nodeBom);
  nodeBom.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->bom())));

  QDomElement nodeSortOrder = element.ownerDocument().createElement(FontOptions::FieldSortOrder);
  nodeFont.appendChild(nodeSortOrder);
  nodeSortOrder.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->sortOrder())));

  QDomElement nodeCodec = element.ownerDocument().createElement(FontOptions::FieldCodec);
  nodeFont.appendChild(nodeCodec);
  nodeCodec.appendChild(element.ownerDocument().createTextNode(this->encoding()));

  QDomElement nodeSkipMissingCharacters = element.ownerDocument().createElement(FontOptions::FieldSkipMissingCharacters);
  nodeFont.appendChild(nodeSkipMissingCharacters);
  nodeSkipMissingCharacters.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->skipMissingCharacters())));

  QDomElement nodeEscapedCharacters = element.ownerDocument().createElement(FontOptions::FieldEscapedCharacters);
  nodeFont.appendChild(nodeEscapedCharacters);
  nodeEscapedCharacters.appendChild(element.ownerDocument().createTextNode(this->escapedCharacters()));

  QDomElement nodeEscapePrefix = element.ownerDocument().createElement(FontOptions::FieldEscapePrefix);
  nodeFont.appendChild(nodeEscapePrefix);
  nodeEscapePrefix.appendChild(element.ownerDocument().createTextNode(this->escapePrefix()));

  QDomElement nodeEscapeSuffix = element.ownerDocument().createElement(FontOptions::FieldEscapeSuffix);
  nodeFont.appendChild(nodeEscapeSuffix);
  nodeEscapeSuffix.appendChild(element.ownerDocument().createTextNode(this->escapeSuffix()));

  QDomElement nodeCompactGlyphs = element.ownerDocument().createElement(FontOptions::FieldCompactGlyphs);
  nodeFont.appendChild(nodeCompactGlyphs);
  nodeCompactGlyphs.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->compactGlyphs())));
}

QString FontOptions::groupName() const
{
  return FontOptions::GroupName;
}

const QStringList &FontOptions::encodings()
{
  static QStringList result;

  if (result.isEmpty()) {
    QList<QByteArray> codecs = QTextCodec::availableCodecs();

    for (int i = 0; i < codecs.length(); i++) {
      result.append(QString(codecs.at(i)));
    }

    result.sort();
  }

  return result;
}

} // namespace Presets
} // namespace Settings
