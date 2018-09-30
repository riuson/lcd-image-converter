/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "reorderingoptions.h"

#include <QVector>
#include <QSettings>
#include <QtXml>
#include <QDomDocument>

namespace Settings
{
namespace Presets
{

const QString ReorderingOptions::GroupName = QString("reordering");
const QString ReorderingOptions::FieldOperations = QString("operations");
const QString ReorderingOptions::FieldOperation = QString("operation");
const QString ReorderingOptions::FieldMask = QString("mask");
const QString ReorderingOptions::FieldShift = QString("shift");
const QString ReorderingOptions::FieldLeft = QString("left");

ReorderingOptions::ReorderingOptions(QObject *parent) :
  QObject(parent)
{
  this->mOperations = new QVector<quint32>();
}

ReorderingOptions::~ReorderingOptions()
{
  delete this->mOperations;
}

int ReorderingOptions::operationsCount() const
{
  return (this->mOperations->size()) / 2;
}

void ReorderingOptions::operation(int index, quint32 *mask, int *shift, bool *left) const
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

void ReorderingOptions::operationAdd(quint32 mask, int shift, bool left)
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

void ReorderingOptions::operationRemove(int index)
{
  if (index < this->operationsCount()) {
    index *= 2;
    this->mOperations->remove(index + 1);
    this->mOperations->remove(index);
  }

  emit this->changed();
}

void ReorderingOptions::operationsRemoveAll()
{
  for (int i = this->operationsCount() - 1; i >= 0; i--) {
    this->operationRemove(i);
  }

  emit this->changed();
}

void ReorderingOptions::operationReplace(int index, quint32 mask, int shift, bool left)
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

bool ReorderingOptions::load(QSettings *settings)
{
  bool result = true;// this option not implemented in versions < 2013-01-04

  settings->beginGroup(ReorderingOptions::GroupName);

  this->operationsRemoveAll();

  int iOperations = settings->beginReadArray(ReorderingOptions::FieldOperations);

  for (int i = 0; i < iOperations; i++) {
    settings->setArrayIndex(i);

    QString sMask = settings->value(ReorderingOptions::FieldMask, QString("00000000")).toString();
    quint32 uMask, uShift, uLeft;

    if (result) {
      uMask = sMask.toUInt(&result, 16);
    }

    if (result) {
      uShift = settings->value(ReorderingOptions::FieldShift, uint(0)).toUInt(&result);
    }

    if (result) {
      uLeft = settings->value(ReorderingOptions::FieldLeft, uint(0)).toUInt(&result);
    }

    if (result) {
      this->operationAdd(uMask, uShift, uLeft != 0);
    }
  }

  settings->endArray();

  settings->endGroup();

  return result;
}

bool ReorderingOptions::loadXmlElement(QDomElement element)
{
  bool result = false;

  QDomNode nodeSett = element.firstChild();

  while (!nodeSett.isNull()) {
    QDomElement e = nodeSett.toElement();

    if (e.tagName() == ReorderingOptions::GroupName) {
      break;
    }

    nodeSett = nodeSett.nextSibling();
  }

  if (nodeSett.isNull()) {
    return result;
  }

  QDomNode nodeValue = nodeSett.firstChild();

  while (!nodeValue.isNull()) {
    QDomElement e = nodeValue.toElement();

    if (!e.isNull()) {
      if (e.tagName() == ReorderingOptions::FieldOperations) {
        QDomNode nodeOperation = e.firstChild();
        this->operationsRemoveAll();

        while (!nodeOperation.isNull()) {
          QDomNode nodeOperationData = nodeOperation.firstChild();
          quint32 uMask = 0, uShift = 0, uLeft = 0;

          while (!nodeOperationData.isNull()) {
            e = nodeOperationData.toElement();

            if (e.tagName() == ReorderingOptions::FieldMask) {
              QString str = e.text();
              uMask = str.toUInt(&result, 16);
            }

            if (e.tagName() == ReorderingOptions::FieldShift) {
              QString str = e.text();
              uShift = str.toUInt(&result);
            }

            if (e.tagName() == ReorderingOptions::FieldLeft) {
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

  return result;
}

void ReorderingOptions::save(QSettings *settings)
{
  settings->beginGroup(ReorderingOptions::GroupName);

  settings->beginWriteArray(ReorderingOptions::FieldOperations);

  for (int i = 0; i < this->operationsCount(); i++) {
    quint32 uMask;
    int iShift;
    bool bLeft;
    this->operation(i, &uMask, &iShift, &bLeft);

    settings->setArrayIndex(i);
    settings->setValue(ReorderingOptions::FieldMask,  QString("%1").arg(uMask, 8, 16, QChar('0')));
    settings->setValue(ReorderingOptions::FieldShift, QString("%1").arg(iShift));
    settings->setValue(ReorderingOptions::FieldLeft,  QString("%1").arg((int)bLeft));
  }

  settings->endArray();

  settings->endGroup();
}

void ReorderingOptions::saveXmlElement(QDomElement element)
{
  QDomElement nodeReordering = element.ownerDocument().createElement(ReorderingOptions::GroupName);
  element.appendChild(nodeReordering);

  QDomElement nodeOperations = element.ownerDocument().createElement(ReorderingOptions::FieldOperations);
  nodeReordering.appendChild(nodeOperations);
  nodeOperations.setAttribute("count", this->operationsCount());

  for (int i = 0; i < this->operationsCount(); i++) {
    quint32 uMask;
    int iShift;
    bool bLeft;
    this->operation(i, &uMask, &iShift, &bLeft);

    QDomElement nodeOperation = element.ownerDocument().createElement(ReorderingOptions::FieldOperation);
    nodeOperations.appendChild(nodeOperation);
    nodeOperation.setAttribute("index", i);

    QDomElement nodeMask = element.ownerDocument().createElement(ReorderingOptions::FieldMask);
    nodeOperation.appendChild(nodeMask);
    nodeMask.appendChild(element.ownerDocument().createTextNode(QString("%1").arg(uMask, 8, 16, QChar('0'))));

    QDomElement nodeShift = element.ownerDocument().createElement(ReorderingOptions::FieldShift);
    nodeOperation.appendChild(nodeShift);
    nodeShift.appendChild(element.ownerDocument().createTextNode(QString("%1").arg(iShift)));

    QDomElement nodeLeft = element.ownerDocument().createElement(ReorderingOptions::FieldLeft);
    nodeOperation.appendChild(nodeLeft);
    nodeLeft.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)bLeft)));
  }
}

QString ReorderingOptions::groupName() const
{
  return ReorderingOptions::GroupName;
}

} // namespace Presets
} // namespace Settings
