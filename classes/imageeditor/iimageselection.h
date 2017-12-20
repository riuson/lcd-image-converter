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

#ifndef IIMAGESELECTION_H
#define IIMAGESELECTION_H

#include <QPainterPath>

namespace ImageEditor
{

class IImageSelection
{
public:
  virtual ~IImageSelection() { }

  virtual const QPainterPath &selectedPath() const = 0;

signals:
  virtual void selectionChanged(const QPainterPath &value) = 0;
};

} // namespace ImageEditor

Q_DECLARE_INTERFACE (ImageEditor::IImageSelection,
                     "riuson.lcd-image-converter/1.0"
                    )

#endif // IIMAGESELECTION_H
