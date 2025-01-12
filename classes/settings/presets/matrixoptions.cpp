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

#include "matrixoptions.h"

#include <QDomDocument>
#include <QSettings>
#include <QVector>
#include <QtXml>

namespace Settings
{
namespace Presets
{

const QString MatrixOptions::GroupName = QString("matrix");
const QString MatrixOptions::FieldMaskUsed = QString("maskUsed");
const QString MatrixOptions::FieldMaskAnd = QString("maskAnd");
const QString MatrixOptions::FieldMaskOr = QString("maskOr");
const QString MatrixOptions::FieldMaskFill = QString("maskFill");
const QString MatrixOptions::FieldOperations = QString("operations");
const QString MatrixOptions::FieldOperation = QString("operation");
const QString MatrixOptions::FieldMask = QString("mask");
const QString MatrixOptions::FieldShift = QString("shift");
const QString MatrixOptions::FieldLeft = QString("left");

MatrixOptions::MatrixOptions(QObject* parent) : QObject(parent)
{
  this->mOperations = new QVector<quint32>();
  this->mMaskUsed = 0x00ffffff;
  this->mMaskAnd = 0xffffffff;
  this->mMaskOr = 0x00000000;
  this->mMaskFill = 0xffffffff;
}

MatrixOptions::~MatrixOptions() { delete this->mOperations; }

quint32 MatrixOptions::maskUsed() const { return (this->mMaskUsed != 0) ? this->mMaskUsed : 1; }

quint32 MatrixOptions::maskAnd() const { return (this->mMaskAnd != 0) ? this->mMaskAnd : 1; }

quint32 MatrixOptions::maskOr() const { return this->mMaskOr; }

quint32 MatrixOptions::maskFill() const { return (this->mMaskFill != 0) ? this->mMaskFill : 1; }

void MatrixOptions::setMaskUsed(quint32 value)
{
  if (value == 0) {
    value = 1;
  }

  if (this->mMaskUsed != value) {
    this->mMaskUsed = value;
    emit this->changed();
  }
}

void MatrixOptions::setMaskAnd(quint32 value)
{
  if (value == 0) {
    value = 1;
  }

  if (this->mMaskAnd != value) {
    this->mMaskAnd = value;
    emit this->changed();
  }
}

void MatrixOptions::setMaskOr(quint32 value)
{
  if (this->mMaskOr != value) {
    this->mMaskOr = value;
    emit this->changed();
  }
}

void MatrixOptions::setMaskFill(quint32 value)
{
  if (value == 0) {
    value = 0xffffffff;
  }

  if (this->mMaskFill != value) {
    this->mMaskFill = value;
    emit this->changed();
  }
}

int MatrixOptions::operationsCount() const { return (this->mOperations->size()) / 2; }

void MatrixOptions::operation(int index, quint32* mask, int* shift, bool* left) const
{
  *mask = 0;
  *shift = 0;
  *left = false;

  if (index < this->operationsCount()) {
    index = (index * 2);

    *mask = this->mOperations->at(index);
    *shift = (this->mOperations->at(index + 1) & 0x0000001f);
    *left = (this->mOperations->at(index + 1) & 0x80000000) != 0;
  }
}

void MatrixOptions::operationAdd(quint32 mask, int shift, bool left)
{
  shift = qAbs(shift);

  this->mOperations->append(mask);

  if (left) {
    this->mOperations->append(shift | 0x80000000);
  } else {
    this->mOperations->append(shift);
  }

  emit this->changed();
}

void MatrixOptions::operationRemove(int index)
{
  if (index < this->operationsCount()) {
    index *= 2;
    this->mOperations->remove(index + 1);
    this->mOperations->remove(index);
  }

  emit this->changed();
}

void MatrixOptions::operationsRemoveAll()
{
  for (int i = this->operationsCount() - 1; i >= 0; i--) {
    this->operationRemove(i);
  }

  emit this->changed();
}

void MatrixOptions::operationReplace(int index, quint32 mask, int shift, bool left)
{
  if (index < this->operationsCount()) {
    index *= 2;

    this->mOperations->replace(index, mask);

    if (left) {
      this->mOperations->replace(index + 1, shift | 0x80000000);
    } else {
      this->mOperations->replace(index + 1, shift);
    }
  }

  emit this->changed();
}

bool MatrixOptions::load(QSettings* settings)
{
  bool result = false;

  settings->beginGroup(MatrixOptions::GroupName);

  quint32 uMaskUsed = 0, uMaskAnd = 0, uMaskOr = 0, uMaskFill = 0;

  QString sMaskUsed = settings->value(MatrixOptions::FieldMaskUsed, QString("ffffffff")).toString();
  QString sMaskAnd = settings->value(MatrixOptions::FieldMaskAnd, QString("ffffffff")).toString();
  QString sMaskOr = settings->value(MatrixOptions::FieldMaskOr, QString("00000000")).toString();
  QString sMaskFill = settings->value(MatrixOptions::FieldMaskFill, QString("ffffffff")).toString();

  uMaskUsed = sMaskUsed.toUInt(&result, 16);

  if (result) {
    uMaskAnd = sMaskAnd.toUInt(&result, 16);
  }

  if (result) {
    uMaskOr = sMaskOr.toUInt(&result, 16);
  }

  if (result) {
    uMaskFill = sMaskFill.toUInt(&result, 16);
  }

  if (result) {
    this->setMaskUsed(uMaskUsed);
    this->setMaskAnd(uMaskAnd);
    this->setMaskOr(uMaskOr);
    this->setMaskFill(uMaskFill);

    this->operationsRemoveAll();

    int iOperations = settings->beginReadArray(MatrixOptions::FieldOperations);

    for (int i = 0; i < iOperations; i++) {
      settings->setArrayIndex(i);

      QString sMask = settings->value(MatrixOptions::FieldMask, QString("00000000")).toString();
      quint32 uMask, uShift, uLeft;

      if (result) {
        uMask = sMask.toUInt(&result, 16);
      }

      if (result) {
        uShift = settings->value(MatrixOptions::FieldShift, uint(0)).toUInt(&result);
      }

      if (result) {
        uLeft = settings->value(MatrixOptions::FieldLeft, uint(0)).toUInt(&result);
      }

      if (result) {
        this->operationAdd(uMask, uShift, uLeft != 0);
      }
    }

    settings->endArray();
  }

  settings->endGroup();

  return result;
}

bool MatrixOptions::loadXmlElement(QDomElement element)
{
  bool result = false;

  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == MatrixOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return result;
  }

  quint32 uMaskUsed = 0xffffffff, uMaskAnd = 0xffffffff, uMaskOr = 0, uMaskFill = 0xffffffff;

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == MatrixOptions::FieldMaskUsed) {
        QString str = e.text();
        uMaskUsed = str.toUInt(&result, 16);
      }

      if (e.tagName() == MatrixOptions::FieldMaskAnd) {
        QString str = e.text();
        uMaskAnd = str.toUInt(&result, 16);
      }

      if (e.tagName() == MatrixOptions::FieldMaskOr) {
        QString str = e.text();
        uMaskOr = str.toUInt(&result, 16);
      }

      if (e.tagName() == MatrixOptions::FieldMaskFill) {
        QString str = e.text();
        uMaskFill = str.toUInt(&result, 16);
      }

      if (e.tagName() == MatrixOptions::FieldOperations) {
        QDomNode nodeOperation = e.firstChild();
        this->operationsRemoveAll();

        while (!nodeOperation.isNull()) {
          QDomNode nodeOperationData = nodeOperation.firstChild();
          quint32 uMask = 0, uShift = 0, uLeft = 0;

          while (!nodeOperationData.isNull()) {
            e = nodeOperationData.toElement();

            if (e.tagName() == MatrixOptions::FieldMask) {
              QString str = e.text();
              uMask = str.toUInt(&result, 16);
            }

            if (e.tagName() == MatrixOptions::FieldShift) {
              QString str = e.text();
              uShift = str.toUInt(&result);
            }

            if (e.tagName() == MatrixOptions::FieldLeft) {
              QString str = e.text();
              uLeft = str.toUInt(&result);
            }

            if (!result) {
              break;
            }

            nodeOperationData = nodeOperationData.nextSibling();
          }

          this->operationAdd(uMask, uShift, uLeft != 0);
          nodeOperation = nodeOperation.nextSibling();
        }
      }

      if (!result) {
        break;
      }
    }

    nodeValue = nodeValue.nextSibling();
  }

  if (result) {
    this->setMaskUsed(uMaskUsed);
    this->setMaskAnd(uMaskAnd);
    this->setMaskOr(uMaskOr);
    this->setMaskFill(uMaskFill);
  }

  return result;
}

void MatrixOptions::save(QSettings* settings)
{
  settings->beginGroup(MatrixOptions::GroupName);

  settings->setValue(MatrixOptions::FieldMaskUsed, QString("%1").arg(this->maskUsed(), 8, 16, QChar('0')));
  settings->setValue(MatrixOptions::FieldMaskAnd, QString("%1").arg(this->maskAnd(), 8, 16, QChar('0')));
  settings->setValue(MatrixOptions::FieldMaskOr, QString("%1").arg(this->maskOr(), 8, 16, QChar('0')));
  settings->setValue(MatrixOptions::FieldMaskFill, QString("%1").arg(this->maskFill(), 8, 16, QChar('0')));

  settings->beginWriteArray(MatrixOptions::FieldOperations);

  for (int i = 0; i < this->operationsCount(); i++) {
    quint32 uMask;
    int iShift;
    bool bLeft;
    this->operation(i, &uMask, &iShift, &bLeft);

    settings->setArrayIndex(i);
    settings->setValue(MatrixOptions::FieldMask, QString("%1").arg(uMask, 8, 16, QChar('0')));
    settings->setValue(MatrixOptions::FieldShift, QString("%1").arg(iShift));
    settings->setValue(MatrixOptions::FieldLeft, QString("%1").arg((int)bLeft));
  }

  settings->endArray();

  settings->endGroup();
}

void MatrixOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodeMatrix = element.ownerDocument().createElement(MatrixOptions::GroupName);
  element.appendChild(nodeMatrix);

  QDomElement nodeMaskUsed = element.ownerDocument().createElement(MatrixOptions::FieldMaskUsed);
  nodeMatrix.appendChild(nodeMaskUsed);
  nodeMaskUsed.appendChild(
      element.ownerDocument().createTextNode(QString("%1").arg(this->maskUsed(), 8, 16, QChar('0'))));

  QDomElement nodeMaskAnd = element.ownerDocument().createElement(MatrixOptions::FieldMaskAnd);
  nodeMatrix.appendChild(nodeMaskAnd);
  nodeMaskAnd.appendChild(
      element.ownerDocument().createTextNode(QString("%1").arg(this->maskAnd(), 8, 16, QChar('0'))));

  QDomElement nodeMaskOr = element.ownerDocument().createElement(MatrixOptions::FieldMaskOr);
  nodeMatrix.appendChild(nodeMaskOr);
  nodeMaskOr.appendChild(element.ownerDocument().createTextNode(QString("%1").arg(this->maskOr(), 8, 16, QChar('0'))));

  QDomElement nodeMaskFill = element.ownerDocument().createElement(MatrixOptions::FieldMaskFill);
  nodeMatrix.appendChild(nodeMaskFill);
  nodeMaskFill.appendChild(
      element.ownerDocument().createTextNode(QString("%1").arg(this->maskFill(), 8, 16, QChar('0'))));

  QDomElement nodeOperations = element.ownerDocument().createElement(MatrixOptions::FieldOperations);
  nodeMatrix.appendChild(nodeOperations);
  nodeOperations.setAttribute("count", this->operationsCount());

  for (int i = 0; i < this->operationsCount(); i++) {
    quint32 uMask;
    int iShift;
    bool bLeft;
    this->operation(i, &uMask, &iShift, &bLeft);

    QDomElement nodeOperation = element.ownerDocument().createElement(MatrixOptions::FieldOperation);
    nodeOperations.appendChild(nodeOperation);
    nodeOperation.setAttribute("index", i);

    QDomElement nodeMask = element.ownerDocument().createElement(MatrixOptions::FieldMask);
    nodeOperation.appendChild(nodeMask);
    nodeMask.appendChild(element.ownerDocument().createTextNode(QString("%1").arg(uMask, 8, 16, QChar('0'))));

    QDomElement nodeShift = element.ownerDocument().createElement(MatrixOptions::FieldShift);
    nodeOperation.appendChild(nodeShift);
    nodeShift.appendChild(element.ownerDocument().createTextNode(QString("%1").arg(iShift)));

    QDomElement nodeLeft = element.ownerDocument().createElement(MatrixOptions::FieldLeft);
    nodeOperation.appendChild(nodeLeft);
    nodeLeft.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)bLeft)));
  }
}

QString MatrixOptions::groupName() const { return MatrixOptions::GroupName; }

} // namespace Presets
} // namespace Settings
