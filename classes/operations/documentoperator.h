/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#ifndef DOCUMENTOPERATOR_H
#define DOCUMENTOPERATOR_H

#include <QObject>
#include <QStringList>

class IDocument;

namespace Operations
{
class IOperation;

class DocumentOperator : public QObject
{
  Q_OBJECT
public:
  explicit DocumentOperator(QObject *parent = 0);
  void setKeys(const QStringList keys);
  void apply(IDocument *doc, IOperation &operation);

private:
  QStringList mSelectedKeys;
};

}

#endif // DOCUMENTOPERATOR_H
