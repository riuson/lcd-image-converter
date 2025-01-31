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

#include "unicodeblocksmodel.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>

namespace AppUI
{
namespace Fonts
{

UnicodeBlock::UnicodeBlock(const QString& name, quint32 firstCode, quint32 lastCode)
{
  this->mName = name;
  this->mFirstCode = firstCode;
  this->mLastCode = lastCode;
}

const QString& UnicodeBlock::name() const { return this->mName; }

quint32 UnicodeBlock::firstCode() const { return this->mFirstCode; }

quint32 UnicodeBlock::lastCode() const { return this->mLastCode; }

UnicodeBlocksModel::UnicodeBlocksModel(QObject* parent) : QAbstractListModel(parent)
{
  quint32 min = 0, max = 0;

  QFile data(":/font/unicode-blocks");

  if (data.open(QFile::ReadOnly)) {
    QRegExp expCode("U\\+([0123456789cbdefABCDEF]+)");

    QTextStream stream(&data);
    QString line;

    do {
      line = stream.readLine();

      bool ok;

      int index1 = expCode.indexIn(line, 0);
      QString cap1 = expCode.cap(1);
      quint64 code1 = cap1.toULong(&ok, 16);

      int index2 = expCode.indexIn(line, index1 + 1);
      QString cap2 = expCode.cap(1);
      quint64 code2 = cap2.toULong(&ok, 16);

      if (code1 > 0xfffful || code2 > 0xfffful) {
        continue;
      }

      index1 = line.indexOf("\t");
      index2 = line.indexOf("\t", index1 + 1);
      QString name = line.mid(index1 + 1, index2 - index1 - 1);
      name = name.simplified();

      UnicodeBlock* block = new UnicodeBlock(name, code1, code2);
      this->mList.append(block);

      if (min > code1) {
        min = code1;
      }

      if (max < code2) {
        max = code2;
      }

    } while (!line.isNull());
  }

  UnicodeBlock* firstBlock = new UnicodeBlock(tr("All"), min, max);
  this->mList.insert(0, firstBlock);
}

QVariant UnicodeBlocksModel::data(const QModelIndex& index, int role) const
{
  QVariant result = QVariant();

  if (!index.isValid()) {
    return QVariant();
  }

  const UnicodeBlock* block = this->mList.at(index.row());

  if (role == Qt::DisplayRole) {
    result = block->name();
  } else if (role == FirstCodeRole) {
    result = block->firstCode();
  } else if (role == LastCodeRole) {
    result = block->lastCode();
  }

  return result;
}

int UnicodeBlocksModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);

  return this->mList.length();
}

} // namespace Fonts
} // namespace AppUI
