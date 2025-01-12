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

#ifndef IOPERATION_H
#define IOPERATION_H

namespace Data
{
namespace Containers
{
class IDocument;
}
} // namespace Data

namespace Operations
{
class IOperation
{
public:
  virtual ~IOperation() {}

  virtual bool prepare(const Data::Containers::IDocument* doc, const QStringList& keys) = 0;
  virtual void applyDocument(Data::Containers::IDocument* doc, const QStringList& keys) = 0;
  virtual void applyItem(Data::Containers::IDocument* doc, const QString& itemKey) = 0;
};
} // namespace Operations

Q_DECLARE_INTERFACE(Operations::IOperation, "riuson.lcd-image-converter/1.0")

#endif // IOPERATION_H
