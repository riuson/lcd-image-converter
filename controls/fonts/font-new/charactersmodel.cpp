/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#include "charactersmodel.h"
#include <QPalette>

CharactersModel::CharactersModel(QObject *parent) :
  QAbstractItemModel(parent)
{
  this->mDesiredCode1 = 0;
  this->mDesiredCode2 = 0;
  this->mResultCode1 = 0;
  this->mResultCode2 = 0;
}

int CharactersModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  int result = (this->mResultCode2 + 1 - this->mResultCode1) / 16;

  return result;
}

int CharactersModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)

  return 16;
}

QVariant CharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  QVariant result;

  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Vertical) {
      int row = (section) * 16;
      row += this->mResultCode1;

      result = QString("%1").arg(row, 0, 16);
    } else {
      result = QString("%1").arg((section), 2, 16);
    }
  }

  if (role == Qt::TextAlignmentRole) {
    if (orientation == Qt::Vertical) {
      result = (int)Qt::Alignment(Qt::AlignRight | Qt::AlignVCenter);
    } else if (orientation == Qt::Horizontal) {
      result = Qt::AlignCenter;
    }
  }

  return result;
}

QVariant CharactersModel::data(const QModelIndex &index, int role) const
{
  QVariant result = QVariant();

  if (role == Qt::DisplayRole) {
    if (index.isValid()) {
      quint32 code = index.column() + index.row() * 16;
      code += this->mResultCode1;

      if (code >= this->mDesiredCode1 && code <= this->mDesiredCode2) {
        result = QString(QChar(code));
      }
    }
  } else if (role == Qt::BackgroundRole) {
    if (index.isValid()) {
      quint32 code = index.column() + index.row() * 16;
      code += this->mResultCode1;

      if (code < this->mDesiredCode1 || code > this->mDesiredCode2) {
        QPalette palette;
        result = palette.color(QPalette::Disabled, QPalette::Window);
      }
    }
  }

  if (role == Qt::TextAlignmentRole) {
    result = Qt::AlignCenter;
  }

  return result;
}

QModelIndex CharactersModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return this->createIndex(row, column);
}

QModelIndex CharactersModel::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
  return QModelIndex();
}

void CharactersModel::setCodesRange(quint32 first, quint32 last)
{
  this->beginResetModel();

  this->mDesiredCode1 = first;
  this->mDesiredCode2 = last;

  this->mResultCode1 = first & 0xfffffff0;
  this->mResultCode2 = last | 0x0000000f;

  this->endResetModel();
}

