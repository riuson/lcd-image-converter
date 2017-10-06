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

#ifndef EDITOR_H
#define EDITOR_H

#include <QObject>
#include "iimageeditor.h"

class QColor;

namespace ImageEditor
{
class WindowEditor;

class Editor : public QObject, public IImageEditor
{
  Q_OBJECT
  Q_INTERFACES(ImageEditor::IImageEditor)

public:
  explicit Editor(QObject *parent = 0);
  ~Editor();

  QWidget *widget() const Q_DECL_OVERRIDE;

  const QImage *image() const Q_DECL_OVERRIDE;
  void setImage(const QImage *value) Q_DECL_OVERRIDE;

  int scale() const Q_DECL_OVERRIDE;

signals:
  void imageChanged(const QImage *value) Q_DECL_OVERRIDE;
  void scaleChanged(int value) Q_DECL_OVERRIDE;
  void mouseMoved(const QPoint *value) Q_DECL_OVERRIDE;

signals:

public slots:

private:
  WindowEditor *mWidget;

private slots:
  void on_imageChanged();
  void on_mouseMove(const QPoint *point);
  void on_scaleChanged(int value);
};
}

#endif // EDITOR_H
