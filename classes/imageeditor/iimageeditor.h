/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#ifndef IIMAGEEDITOR_H
#define IIMAGEEDITOR_H

#include <QImage>
#include <QPoint>
#include <QWidget>

namespace ImageEditor
{
class IImageEditor
{
public:
  virtual ~IImageEditor() { }

  virtual QWidget *widget() const = 0;

  virtual const QImage *image() const = 0;
  virtual void setImage(const QImage *_value) = 0;

  virtual int scale() const = 0;

signals:
  virtual void imageChanged(const QImage *_image) = 0;
  virtual void scaleChanged(int _scale) = 0;
  virtual void mouseMoved(const QPoint *_point) = 0;
};
} // namespace ImageEditor

Q_DECLARE_INTERFACE (ImageEditor::IImageEditor,
                     "riuson.lcd-image-converter/1.0"
                    )

#endif // IIMAGEEDITOR_H
