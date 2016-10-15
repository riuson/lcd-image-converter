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

#include "prepareoptions.h"

#include <QSettings>
#include <QBuffer>
#include <QtXml>
#include <QDomDocument>

const QString PrepareOptions::GroupName = QString("prepare");
const QString PrepareOptions::FieldConvType = QString("convType");
const QString PrepareOptions::FieldMonoType = QString("monoType");
const QString PrepareOptions::FieldEdge = QString("edge");
const QString PrepareOptions::FieldScanMain = QString("scanMain");
const QString PrepareOptions::FieldScanSub = QString("scanSub");
const QString PrepareOptions::FieldInverse = QString("inverse");
const QString PrepareOptions::FieldBandScanning = QString("bandScanning");
const QString PrepareOptions::FieldBandWidth = QString("bandWidth");
const QString PrepareOptions::FieldUseCustomScript = QString("useCustomScript");
const QString PrepareOptions::FieldCustomScript = QString("customScript");

PrepareOptions::PrepareOptions(QObject *parent) :
  QObject(parent)
{
  this->mConvType = ConversionTypeMonochrome;
  this->mMonoType = MonochromeTypeDiffuseDither;
  this->mEdge = 128;
  this->mScanMain = TopToBottom;
  this->mScanSub = Forward;
  this->mInverse = false;
  this->mBandScanning = false;
  this->mBandWidth = 0;
  this->mUseCustomScript = false;
  this->mCustomScript = QString();
}

ConversionType PrepareOptions::convType() const
{
  if (this->mConvType <= ConversionTypeColor) {
    return this->mConvType;
  }

  return ConversionTypeColor;
}

MonochromeType PrepareOptions::monoType() const
{
  if (this->mMonoType <= MonochromeTypeThresholdDither) {
    return this->mMonoType;
  }

  return MonochromeTypeThresholdDither;
}

int PrepareOptions::edge() const
{
  if (this->mEdge <= 255) {
    return this->mEdge;
  }

  return 128;
}

ScanMainDirection PrepareOptions::scanMain() const
{
  if (this->mScanMain <= RightToLeft) {
    return this->mScanMain;
  }

  return TopToBottom;
}

ScanSubDirection PrepareOptions::scanSub() const
{
  if (this->mScanSub <= Backward) {
    return this->mScanSub;
  }

  return Forward;
}

bool PrepareOptions::inverse() const
{
  return this->mInverse;
}

bool PrepareOptions::bandScanning() const
{
  return this->mBandScanning;
}

int PrepareOptions::bandWidth() const
{
  if (this->mBandWidth < 1) {
    return 1;
  }

  return this->mBandWidth;
}

bool PrepareOptions::useCustomScript() const
{
  return this->mUseCustomScript;
}

QString PrepareOptions::customScript() const
{
  return this->mCustomScript;
}

void PrepareOptions::setConvType(ConversionType value)
{
  if (this->mConvType != value) {
    if (value < ConversionTypeMonochrome || value > ConversionTypeColor) {
      value = ConversionTypeColor;
    }

    this->mConvType = value;

    emit this->changed();
  }
}

void PrepareOptions::setMonoType(MonochromeType value)
{
  if (this->mMonoType != value) {
    if (value < MonochromeTypeEdge || value > MonochromeTypeThresholdDither) {
      value = MonochromeTypeDiffuseDither;
    }

    this->mMonoType = value;

    emit this->changed();
  }
}

void PrepareOptions::setEdge(int value)
{
  if (this->mEdge != value) {
    if (value < 0 || value > 255) {
      value = 128;
    }

    this->mEdge = value;

    emit this->changed();
  }
}

void PrepareOptions::setScanMain(ScanMainDirection value)
{
  if (this->mScanMain != value) {
    if (value < TopToBottom || value > RightToLeft) {
      value = TopToBottom;
    }

    this->mScanMain = value;

    emit this->changed();
  }
}

void PrepareOptions::setScanSub(ScanSubDirection value)
{
  if (this->mScanSub != value) {
    this->mScanSub = value;

    emit this->changed();
  }
}

void PrepareOptions::setInverse(bool value)
{
  if (this->mInverse != value) {
    this->mInverse = value;

    emit this->changed();
  }
}

void PrepareOptions::setBandScanning(bool value)
{
  if (this->mBandScanning != value) {
    this->mBandScanning = value;
    emit this->changed();
  }
}

void PrepareOptions::setBandWidth(int value)
{
  if (this->mBandWidth != value && value > 0) {
    this->mBandWidth = value;

    emit this->changed();
  }
}

void PrepareOptions::setUseCustomScript(bool value)
{
  if (this->mUseCustomScript != value) {
    this->mUseCustomScript = value;
    emit this->changed();
  }
}

void PrepareOptions::setCustomScript(const QString &value)
{
  if (this->mCustomScript != value) {
    this->mCustomScript = value;
    emit this->changed();
  }
}

const QString &PrepareOptions::convTypeName() const
{
  static const QString names[] = {
    "Monochrome",
    "Grayscale",
    "Color",
    "???"
  };

  switch (this->convType()) {
    case ConversionTypeMonochrome:
      return names[0];

    case ConversionTypeGrayscale:
      return names[1];

    case ConversionTypeColor:
      return names[2];

    default:
      return names[3];
  }
}

const QString &PrepareOptions::monoTypeName() const
{
  static const QString names[] = {
    "Edge",
    "Diffuse Dither",
    "Ordered Dither",
    "Threshold Dither",
    "???"
  };

  switch (this->monoType()) {
    case MonochromeTypeEdge:
      return names[0];

    case MonochromeTypeDiffuseDither:
      return names[1];

    case MonochromeTypeOrderedDither:
      return names[2];

    case MonochromeTypeThresholdDither:
      return names[3];

    default:
      return names[4];
  }
}

bool PrepareOptions::load(QSettings *settings, int version)
{
  bool result = false;

  if (version == 1) {
    quint32 uConvType = 0, uMonoType = 0, uEdge = 0;
    quint32 uScanMain = 0, uScanSub = 0, uInverse = 0;
    quint32 uBandWidth = 1, uBandScanning = 0;
    quint32 uUseCustomScript = 0;
    QString sCustomScript;

    uConvType = settings->value(PrepareOptions::FieldConvType, int(0)).toUInt(&result);

    if (result) {
      uMonoType = settings->value(PrepareOptions::FieldMonoType, int(0)).toUInt(&result);
    }

    if (result) {
      uEdge = settings->value(PrepareOptions::FieldEdge, int(0)).toUInt(&result);
    }

    if (result) {
      uScanMain = settings->value(PrepareOptions::FieldScanMain, int(0)).toUInt(&result);
    }

    if (result) {
      uScanSub = settings->value(PrepareOptions::FieldScanSub, int(0)).toUInt(&result);
    }

    if (result) {
      uInverse = settings->value(PrepareOptions::FieldInverse, int(0)).toUInt(&result);
    }

    if (result) {
      uBandScanning = settings->value(PrepareOptions::FieldBandScanning, false).toBool();
    }

    if (result) {
      uBandWidth = settings->value(PrepareOptions::FieldBandWidth, int(1)).toUInt(&result);
    }

    if (result) {
      uUseCustomScript = settings->value(PrepareOptions::FieldUseCustomScript, false).toBool();
    }

    if (result) {
      QString str = settings->value(PrepareOptions::FieldCustomScript, QString()).toString();
      QByteArray ba = QByteArray::fromBase64(str.toLatin1());
      QBuffer buffer(&ba);
      sCustomScript = QString::fromUtf8(buffer.data());
    }

    if (result) {
      this->setConvType((ConversionType)uConvType);
      this->setMonoType((MonochromeType)uMonoType);
      this->setEdge((int)uEdge);
      this->setScanMain((ScanMainDirection)uScanMain);
      this->setScanSub((ScanSubDirection)uScanSub);
      this->setInverse((bool)uInverse);
      this->setBandScanning((bool)uBandScanning);
      this->setBandWidth((int)uBandWidth);
      this->setUseCustomScript((bool)uUseCustomScript);
      this->setCustomScript(sCustomScript);
    }
  } else if (version == 2) {
    settings->beginGroup(PrepareOptions::GroupName);

    result = this->load(settings, 1);

    settings->endGroup();
  }

  return result;
}

bool PrepareOptions::loadXmlElement(QDomElement element)
{
  bool result = false;

  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == PrepareOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return result;
  }

  quint32 uConvType = 0, uMonoType = 0, uEdge = 0;
  quint32 uScanMain = 0, uScanSub = 0, uInverse = 0;
  quint32 uBandWidth = 1, uBandScanning = 0;
  quint32 uUseCustomScript = 0;
  QString sCustomScript;

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == PrepareOptions::FieldConvType) {
        QString str = e.text();
        uConvType = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldMonoType) {
        QString str = e.text();
        uMonoType = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldEdge) {
        QString str = e.text();
        uEdge = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldScanMain) {
        QString str = e.text();
        uScanMain = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldScanSub) {
        QString str = e.text();
        uScanSub = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldInverse) {
        QString str = e.text();
        uInverse = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldBandScanning) {
        QString str = e.text();
        uBandScanning = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldBandWidth) {
        QString str = e.text();
        uBandWidth = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldUseCustomScript) {
        QString str = e.text();
        uUseCustomScript = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldEdge) {
        QString str = e.text();
        uEdge = str.toUInt(&result);
      }

      if (e.tagName() == PrepareOptions::FieldCustomScript) {
        QDomNode cdataNode = e.firstChild();

        if (cdataNode.isCDATASection()) {
          QDomCDATASection cdataSection = cdataNode.toCDATASection();
          sCustomScript = cdataSection.data();
        }
      }

      if (!result) {
        break;
      }
    }

    nodeValue = nodeValue.nextSibling();
  }

  if (result) {
    this->setConvType((ConversionType)uConvType);
    this->setMonoType((MonochromeType)uMonoType);
    this->setEdge((int)uEdge);
    this->setScanMain((ScanMainDirection)uScanMain);
    this->setScanSub((ScanSubDirection)uScanSub);
    this->setInverse((bool)uInverse);
    this->setBandScanning((bool)uBandScanning);
    this->setBandWidth((int)uBandWidth);
    this->setUseCustomScript((bool)uUseCustomScript);
    this->setCustomScript(sCustomScript);
  }

  return result;
}

void PrepareOptions::save(QSettings *settings)
{
  settings->beginGroup(PrepareOptions::GroupName);

  settings->setValue(PrepareOptions::FieldConvType, QString("%1").arg((int)this->convType()));
  settings->setValue(PrepareOptions::FieldMonoType, QString("%1").arg((int)this->monoType()));
  settings->setValue(PrepareOptions::FieldEdge,     QString("%1").arg((int)this->edge()));
  settings->setValue(PrepareOptions::FieldScanMain, QString("%1").arg((int)this->scanMain()));
  settings->setValue(PrepareOptions::FieldScanSub,  QString("%1").arg((int)this->scanSub()));
  settings->setValue(PrepareOptions::FieldInverse,  QString("%1").arg((int)this->inverse()));
  settings->setValue(PrepareOptions::FieldBandScanning,    QString("%1").arg((int)this->bandScanning()));
  settings->setValue(PrepareOptions::FieldBandWidth,       QString("%1").arg((int)this->bandWidth()));
  settings->setValue(PrepareOptions::FieldUseCustomScript, QString("%1").arg((int)this->useCustomScript()));

  {
    QByteArray array = this->mCustomScript.toUtf8();
    array = array.toBase64();
    QString str = QString::fromLatin1(array);
    settings->setValue("customScript", str);
  }

  settings->endGroup();
}

void PrepareOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodePrepare = element.ownerDocument().createElement(PrepareOptions::GroupName);
  element.appendChild(nodePrepare);

  QDomElement nodeConvType = element.ownerDocument().createElement(PrepareOptions::FieldConvType);
  nodePrepare.appendChild(nodeConvType);
  nodeConvType.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->convType())));

  QDomElement nodeMonoType = element.ownerDocument().createElement(PrepareOptions::FieldMonoType);
  nodePrepare.appendChild(nodeMonoType);
  nodeMonoType.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->monoType())));

  QDomElement nodeEdge = element.ownerDocument().createElement(PrepareOptions::FieldEdge);
  nodePrepare.appendChild(nodeEdge);
  nodeEdge.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->edge())));

  QDomElement nodeScanMain = element.ownerDocument().createElement(PrepareOptions::FieldScanMain);
  nodePrepare.appendChild(nodeScanMain);
  nodeScanMain.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->scanMain())));

  QDomElement nodeScanSub = element.ownerDocument().createElement(PrepareOptions::FieldScanSub);
  nodePrepare.appendChild(nodeScanSub);
  nodeScanSub.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->scanSub())));

  QDomElement nodeInverse = element.ownerDocument().createElement(PrepareOptions::FieldInverse);
  nodePrepare.appendChild(nodeInverse);
  nodeInverse.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->inverse())));

  QDomElement nodeBandScanning = element.ownerDocument().createElement(PrepareOptions::FieldBandScanning);
  nodePrepare.appendChild(nodeBandScanning);
  nodeBandScanning.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->bandScanning())));

  QDomElement nodeBandWidth = element.ownerDocument().createElement(PrepareOptions::FieldBandWidth);
  nodePrepare.appendChild(nodeBandWidth);
  nodeBandWidth.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->bandWidth())));

  QDomElement nodeUseCustomScript = element.ownerDocument().createElement(PrepareOptions::FieldUseCustomScript);
  nodePrepare.appendChild(nodeUseCustomScript);
  nodeUseCustomScript.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->useCustomScript())));

  QDomElement nodeCustomScript = element.ownerDocument().createElement(PrepareOptions::FieldCustomScript);
  nodePrepare.appendChild(nodeCustomScript);
  nodeCustomScript.appendChild(element.ownerDocument().createCDATASection(this->mCustomScript));
}

