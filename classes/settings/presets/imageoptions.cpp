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

#include "imageoptions.h"

#include <QSettings>
#include <QtXml>
#include <QDomDocument>

const QString ImageOptions::GroupName = QString("image");
const QString ImageOptions::FieldBytesOrder = QString("bytesOrder");
const QString ImageOptions::FieldBlockSize = QString("blockSize");
const QString ImageOptions::FieldBlockDefaultOnes = QString("blockDefaultOnes");
const QString ImageOptions::FieldSplitToRows = QString("splitToRows");
const QString ImageOptions::FieldCompressionRle = QString("compressionRle");
const QString ImageOptions::FieldCompressionRleMinLength = QString("compressionRleMinLength");
const QString ImageOptions::FieldBandWidth = QString("bandWidth");
const QString ImageOptions::FieldBlockPrefix = QString("blockPrefix");
const QString ImageOptions::FieldBlockSuffix = QString("blockSuffix");
const QString ImageOptions::FieldBlockDelimiter = QString("blockDelimiter");
const QString ImageOptions::FieldPreviewPrefix = QString("previewPrefix");
const QString ImageOptions::FieldPreviewSuffix = QString("previewSuffix");
const QString ImageOptions::FieldPreviewDelimiter = QString("previewDelimiter");
const QString ImageOptions::FieldPreviewBit0 = QString("previewBit0");
const QString ImageOptions::FieldPreviewBit1 = QString("previewBit1");

ImageOptions::ImageOptions(QObject *parent) :
  QObject(parent)
{
  this->mSplitToRows = true;
  this->mBytesOrder = BytesOrderLittleEndian;
  this->mBlockSize = Data8;
  this->mBlockDefaultOnes = false;
  this->mCompressionRle = false;
  this->mCompressionRleMinLength = 2;
  this->mBlockPrefix = "0x";
  this->mBlockSuffix = "";
  this->mBlockDelimiter = ", ";
  this->mPreviewPrefix = "// ";
  this->mPreviewSuffix = "";
  this->mPreviewDelimiter = "";
  this->mPreviewBit0 = "░";
  this->mPreviewBit1 = "█";
}

bool ImageOptions::splitToRows() const
{
  return this->mSplitToRows;
}

BytesOrder ImageOptions::bytesOrder() const
{
  return this->mBytesOrder;
}

DataBlockSize ImageOptions::blockSize() const
{
  if (this->mBlockSize <= Data32) {
    return this->mBlockSize;
  }

  return Data32;
}

bool ImageOptions::blockDefaultOnes() const
{
  return this->mBlockDefaultOnes;
}

bool ImageOptions::compressionRle() const
{
  return this->mCompressionRle;
}

quint32 ImageOptions::compressionRleMinLength() const
{
  return this->mCompressionRleMinLength;
}

QString ImageOptions::blockPrefix() const
{
  return this->mBlockPrefix;
}

QString ImageOptions::blockSuffix() const
{
  return this->mBlockSuffix;
}

QString ImageOptions::blockDelimiter() const
{
  return this->mBlockDelimiter;
}

QString ImageOptions::previewPrefix() const
{
  return this->mPreviewPrefix;
}

QString ImageOptions::previewSuffix() const
{
  return this->mPreviewSuffix;
}

QString ImageOptions::previewDelimiter() const
{
  return this->mPreviewDelimiter;
}

QString ImageOptions::previewBit0() const
{
  return this->mPreviewBit0;
}

QString ImageOptions::previewBit1() const
{
  return this->mPreviewBit1;
}

void ImageOptions::setSplitToRows(bool value)
{
  this->mSplitToRows = value;

  emit this->changed();
}

void ImageOptions::setBytesOrder(BytesOrder value)
{
  if (value < BytesOrderLittleEndian || value > BytesOrderBigEndian) {
    value = BytesOrderLittleEndian;
  }

  this->mBytesOrder = value;

  emit this->changed();
}

void ImageOptions::setBlockSize(DataBlockSize value)
{
  if (value < Data8 || value > Data32) {
    value = Data32;
  }

  this->mBlockSize = value;

  emit this->changed();
}

void ImageOptions::setBlockDefaultOnes(bool value)
{
  this->mBlockDefaultOnes = value;

  emit this->changed();
}

void ImageOptions::setCompressionRle(bool value)
{
  this->mCompressionRle = value;

  emit this->changed();
}

void ImageOptions::setCompressionRleMinLength(quint32 value)
{
  this->mCompressionRleMinLength = value;

  emit this->changed();
}

void ImageOptions::setBlockPrefix(const QString &value)
{
  this->mBlockPrefix = value;

  emit this->changed();
}

void ImageOptions::setBlockSuffix(const QString &value)
{
  this->mBlockSuffix = value;

  emit this->changed();
}

void ImageOptions::setBlockDelimiter(const QString &value)
{
  this->mBlockDelimiter = value;

  emit this->changed();
}

void ImageOptions::setPreviewPrefix(const QString &value)
{
  this->mPreviewPrefix = value;

  emit this->changed();
}

void ImageOptions::setPreviewSuffix(const QString &value)
{
  this->mPreviewSuffix = value;

  emit this->changed();
}

void ImageOptions::setPreviewDelimiter(const QString &value)
{
  this->mPreviewDelimiter = value;

  emit this->changed();
}

void ImageOptions::setPreviewBit0(const QString &value)
{
  this->mPreviewBit0 = value;

  emit this->changed();
}

void ImageOptions::setPreviewBit1(const QString &value)
{
  this->mPreviewBit1 = value;

  emit this->changed();
}

bool ImageOptions::load(QSettings *settings)
{
  bool result = false;

  settings->beginGroup(ImageOptions::GroupName);

  quint32 uBytesOrder = 0, uBlockSize = 0, uBlockDefaultOnes = 0, uSplitToRows = 0;
  quint32 uCompressionRle = 0, uCompressionRleMinLength = 2;
  QString sBlockPrefix, sBlockSuffix, sBlockDelimiter;
  QString sPreviewPrefix, sPreviewSuffix, sPreviewDelimiter, sPreviewBit0, sPreviewBit1;

  uBlockSize = settings->value(ImageOptions::FieldBlockSize, int(0)).toUInt(&result);

  if (result) {
    uBytesOrder = settings->value(ImageOptions::FieldBytesOrder, int(0)).toUInt(&result);
  }

  if (result) {
    uSplitToRows = settings->value(ImageOptions::FieldSplitToRows, int(1)).toUInt(&result);
  }

  if (result) {
    uCompressionRle = settings->value(ImageOptions::FieldCompressionRle, int(0)).toUInt(&result);
  }

  if (result) {
    uCompressionRleMinLength = settings->value(ImageOptions::FieldCompressionRleMinLength, int(2)).toUInt(&result);
  }

  if (result) {
    uBlockDefaultOnes = settings->value(ImageOptions::FieldBlockDefaultOnes, int(0)).toUInt(&result);
  }

  sBlockPrefix = settings->value(ImageOptions::FieldBlockPrefix, "0x").toString();
  sBlockSuffix = settings->value(ImageOptions::FieldBlockSuffix, "").toString();
  sBlockDelimiter = settings->value(ImageOptions::FieldBlockDelimiter, ", ").toString();

  sBlockPrefix = this->unescapeEmpty(sBlockPrefix);
  sBlockSuffix = this->unescapeEmpty(sBlockSuffix);
  sBlockDelimiter = this->unescapeEmpty(sBlockDelimiter);

  sPreviewPrefix = settings->value(ImageOptions::FieldPreviewPrefix, "// ").toString();
  sPreviewSuffix = settings->value(ImageOptions::FieldPreviewSuffix, "").toString();
  sPreviewDelimiter = settings->value(ImageOptions::FieldPreviewDelimiter, "").toString();
  sPreviewBit0 = settings->value(ImageOptions::FieldPreviewBit0, "░").toString();
  sPreviewBit1 = settings->value(ImageOptions::FieldPreviewBit1, "█").toString();

  sPreviewPrefix = this->unescapeEmpty(sPreviewPrefix);
  sPreviewSuffix = this->unescapeEmpty(sPreviewSuffix);
  sPreviewDelimiter = this->unescapeEmpty(sPreviewDelimiter);
  sPreviewBit0 = this->unescapeEmpty(sPreviewBit0);
  sPreviewBit1 = this->unescapeEmpty(sPreviewBit1);

  if (result) {
    this->setBlockSize((DataBlockSize)uBlockSize);
    this->setBlockDefaultOnes((bool)uBlockDefaultOnes);
    this->setBytesOrder((BytesOrder)uBytesOrder);
    this->setSplitToRows((bool)uSplitToRows);
    this->setCompressionRle((bool)uCompressionRle);
    this->setCompressionRleMinLength(uCompressionRleMinLength);
    this->setBlockPrefix(sBlockPrefix);
    this->setBlockSuffix(sBlockSuffix);
    this->setBlockDelimiter(sBlockDelimiter);
    this->setPreviewPrefix(sPreviewPrefix);
    this->setPreviewSuffix(sPreviewSuffix);
    this->setPreviewDelimiter(sPreviewDelimiter);
    this->setPreviewBit0(sPreviewBit0);
    this->setPreviewBit1(sPreviewBit1);
  }

  settings->endGroup();

  return result;
}

bool ImageOptions::loadXmlElement(QDomElement element)
{
  bool result = false;

  auto readStringFromNode = [this](QDomElement element, const QString elementName, QString * result) {
    if (element.tagName() == elementName) {
      QDomNode dataNode = element.firstChild();

      if (dataNode.isCDATASection()) {
        QDomCDATASection cdataSection = dataNode.toCDATASection();
        QString strValue = cdataSection.data();
        *result = this->unescapeEmpty(strValue);
      } else {
        QString strValue = element.text();
        *result = this->unescapeEmpty(strValue);
      }
    }
  };


  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == ImageOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return result;
  }

  quint32 uBytesOrder = 0, uBlockSize = 0, uBlockDefaultOnes = 0, uSplitToRows = 0;
  quint32 uCompressionRle = 0, uCompressionRleMinLength = 2;
  QString sBlockPrefix = "0x", sBlockSuffix, sBlockDelimiter = ", ";
  QString sPreviewPrefix, sPreviewSuffix, sPreviewDelimiter, sPreviewBit0, sPreviewBit1;

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == ImageOptions::FieldBlockSize) {
        QString str = e.text();
        uBlockSize = str.toUInt(&result);
      }

      if (e.tagName() == ImageOptions::FieldBytesOrder) {
        QString str = e.text();
        uBytesOrder = str.toUInt(&result);
      }

      if (e.tagName() == ImageOptions::FieldSplitToRows) {
        QString str = e.text();
        uSplitToRows = str.toUInt(&result);
      }

      if (e.tagName() == ImageOptions::FieldCompressionRle) {
        QString str = e.text();
        uCompressionRle = str.toUInt(&result);
      }

      if (e.tagName() == ImageOptions::FieldCompressionRleMinLength) {
        QString str = e.text();
        uCompressionRleMinLength = str.toUInt(&result);
      }

      if (e.tagName() == ImageOptions::FieldBlockDefaultOnes) {
        QString str = e.text();
        uBlockDefaultOnes = str.toUInt(&result);
      }

      readStringFromNode(e, ImageOptions::FieldBlockPrefix, &sBlockPrefix);
      readStringFromNode(e, ImageOptions::FieldBlockSuffix, &sBlockSuffix);
      readStringFromNode(e, ImageOptions::FieldBlockDelimiter, &sBlockDelimiter);
      readStringFromNode(e, ImageOptions::FieldPreviewPrefix, &sPreviewPrefix);
      readStringFromNode(e, ImageOptions::FieldPreviewSuffix, &sPreviewSuffix);
      readStringFromNode(e, ImageOptions::FieldPreviewDelimiter, &sPreviewDelimiter);
      readStringFromNode(e, ImageOptions::FieldPreviewBit0, &sPreviewBit0);
      readStringFromNode(e, ImageOptions::FieldPreviewBit1, &sPreviewBit1);

      if (!result) {
        break;
      }
    }

    nodeValue = nodeValue.nextSibling();
  }

  if (result) {
    this->setBlockSize((DataBlockSize)uBlockSize);
    this->setBlockDefaultOnes((bool)uBlockDefaultOnes);
    this->setBytesOrder((BytesOrder)uBytesOrder);
    this->setSplitToRows((bool)uSplitToRows);
    this->setCompressionRle((bool)uCompressionRle);
    this->setCompressionRleMinLength(uCompressionRleMinLength);
    this->setBlockPrefix(sBlockPrefix);
    this->setBlockSuffix(sBlockSuffix);
    this->setBlockDelimiter(sBlockDelimiter);
    this->setPreviewPrefix(sPreviewPrefix);
    this->setPreviewSuffix(sPreviewSuffix);
    this->setPreviewDelimiter(sPreviewDelimiter);
    this->setPreviewBit0(sPreviewBit0);
    this->setPreviewBit1(sPreviewBit1);
  }

  return result;
}

void ImageOptions::save(QSettings *settings)
{
  settings->beginGroup(ImageOptions::GroupName);

  settings->setValue(ImageOptions::FieldBytesOrder,       QString("%1").arg((int)this->bytesOrder()));
  settings->setValue(ImageOptions::FieldBlockSize,        QString("%1").arg((int)this->blockSize()));
  settings->setValue(ImageOptions::FieldBlockDefaultOnes, QString("%1").arg((int)this->blockDefaultOnes()));
  settings->setValue(ImageOptions::FieldSplitToRows,      QString("%1").arg((int)this->splitToRows()));
  settings->setValue(ImageOptions::FieldCompressionRle,   QString("%1").arg((int)this->compressionRle()));
  settings->setValue(ImageOptions::FieldCompressionRleMinLength,   QString("%1").arg((int)this->compressionRleMinLength()));
  settings->setValue(ImageOptions::FieldBlockPrefix,      this->escapeEmpty(this->blockPrefix()));
  settings->setValue(ImageOptions::FieldBlockSuffix,      this->escapeEmpty(this->blockSuffix()));
  settings->setValue(ImageOptions::FieldBlockDelimiter,   this->escapeEmpty(this->blockDelimiter()));
  settings->setValue(ImageOptions::FieldPreviewPrefix,    this->escapeEmpty(this->previewPrefix()));
  settings->setValue(ImageOptions::FieldPreviewSuffix,    this->escapeEmpty(this->previewSuffix()));
  settings->setValue(ImageOptions::FieldPreviewDelimiter, this->escapeEmpty(this->previewDelimiter()));
  settings->setValue(ImageOptions::FieldPreviewBit0,      this->escapeEmpty(this->previewBit0()));
  settings->setValue(ImageOptions::FieldPreviewBit1,      this->escapeEmpty(this->previewBit1()));

  settings->endGroup();
}

void ImageOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodeImage = element.ownerDocument().createElement(ImageOptions::GroupName);
  element.appendChild(nodeImage);

  QDomElement nodeBytesOrder = element.ownerDocument().createElement(ImageOptions::FieldBytesOrder);
  nodeImage.appendChild(nodeBytesOrder);
  nodeBytesOrder.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->bytesOrder())));

  QDomElement nodeBlockSize = element.ownerDocument().createElement(ImageOptions::FieldBlockSize);
  nodeImage.appendChild(nodeBlockSize);
  nodeBlockSize.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->blockSize())));

  QDomElement nodeBlockDefaultOnes = element.ownerDocument().createElement(ImageOptions::FieldBlockDefaultOnes);
  nodeImage.appendChild(nodeBlockDefaultOnes);
  nodeBlockDefaultOnes.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->blockDefaultOnes())));

  QDomElement nodeSplitToRows = element.ownerDocument().createElement(ImageOptions::FieldSplitToRows);
  nodeImage.appendChild(nodeSplitToRows);
  nodeSplitToRows.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->splitToRows())));

  QDomElement nodeCompressionRle = element.ownerDocument().createElement(ImageOptions::FieldCompressionRle);
  nodeImage.appendChild(nodeCompressionRle);
  nodeCompressionRle.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->compressionRle())));

  QDomElement nodeCompressionRleMinLength = element.ownerDocument().createElement(ImageOptions::FieldCompressionRleMinLength);
  nodeImage.appendChild(nodeCompressionRleMinLength);
  nodeCompressionRleMinLength.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->compressionRleMinLength())));

  QDomElement nodeBlockPrefix = element.ownerDocument().createElement(ImageOptions::FieldBlockPrefix);
  nodeImage.appendChild(nodeBlockPrefix);
  nodeBlockPrefix.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->blockPrefix())));

  QDomElement nodeBlockSuffix = element.ownerDocument().createElement(ImageOptions::FieldBlockSuffix);
  nodeImage.appendChild(nodeBlockSuffix);
  nodeBlockSuffix.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->blockSuffix())));

  QDomElement nodeBlockDelimiter = element.ownerDocument().createElement(ImageOptions::FieldBlockDelimiter);
  nodeImage.appendChild(nodeBlockDelimiter);
  nodeBlockDelimiter.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->blockDelimiter())));

  QDomElement nodePreviewPrefix = element.ownerDocument().createElement(ImageOptions::FieldPreviewPrefix);
  nodeImage.appendChild(nodePreviewPrefix);
  nodePreviewPrefix.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->previewPrefix())));

  QDomElement nodePreviewSuffix = element.ownerDocument().createElement(ImageOptions::FieldPreviewSuffix);
  nodeImage.appendChild(nodePreviewSuffix);
  nodePreviewSuffix.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->previewSuffix())));

  QDomElement nodePreviewDelimiter = element.ownerDocument().createElement(ImageOptions::FieldPreviewDelimiter);
  nodeImage.appendChild(nodePreviewDelimiter);
  nodePreviewDelimiter.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->previewDelimiter())));

  QDomElement nodePreviewBit0 = element.ownerDocument().createElement(ImageOptions::FieldPreviewBit0);
  nodeImage.appendChild(nodePreviewBit0);
  nodePreviewBit0.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->previewBit0())));

  QDomElement nodePreviewBit1 = element.ownerDocument().createElement(ImageOptions::FieldPreviewBit1);
  nodeImage.appendChild(nodePreviewBit1);
  nodePreviewBit1.appendChild(element.ownerDocument().createCDATASection(this->escapeEmpty(this->previewBit1())));
}

QString ImageOptions::escapeEmpty(const QString &value) const
{
  if (value.isEmpty()) {
    return "empty";
  }

  return value;
}

QString ImageOptions::unescapeEmpty(const QString &value) const
{
  if (value == "empty") {
    return QString();
  }

  return value;
}

