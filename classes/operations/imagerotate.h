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

#ifndef IMAGEROTATE_H
#define IMAGEROTATE_H

#include <QObject>

#include "ioperation.h"

namespace Operations
{

class ImageRotate : public QObject, public IOperation
{
  Q_OBJECT
  Q_INTERFACES(Operations::IOperation)

public:
  enum class Angle { None, A90, A180, A270 };

  explicit ImageRotate(QObject* parent = 0);
  virtual ~ImageRotate() {}

  bool prepare(const Data::Containers::IDocument* doc, const QStringList& keys) Q_DECL_OVERRIDE;
  void applyDocument(Data::Containers::IDocument* doc, const QStringList& keys) Q_DECL_OVERRIDE;
  void applyItem(Data::Containers::IDocument* doc, const QString& itemKey) Q_DECL_OVERRIDE;

  void setAngle(Angle angle);

private:
  Angle mAngle;
};

} // namespace Operations

#endif // IMAGEROTATE_H
