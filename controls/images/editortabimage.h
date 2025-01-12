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

#ifndef EDITORTABIMAGE_H
#define EDITORTABIMAGE_H

#include <QWidget>

#include "ieditor.h"

namespace Ui
{
class EditorTabImage;
}

#include <QImage>

namespace Data
{
namespace Containers
{
class DataContainer;
class ImageDocument;
class IDocument;
}
}

namespace ImageEditor
{
class Editor;
}

namespace AppUI
{
namespace Status
{
class StatusData;
}

namespace Images
{

class EditorTabImage : public QWidget, public IEditor
{
  Q_OBJECT
  Q_INTERFACES(AppUI::IEditor)

public:
  explicit EditorTabImage(QWidget *parent = 0);
  virtual ~EditorTabImage();

  Data::Containers::IDocument *document() const Q_DECL_OVERRIDE;
  QStringList selectedKeys() const Q_DECL_OVERRIDE;
  AppUI::Status::StatusData *statusData() const Q_DECL_OVERRIDE;
  EditorType type() const Q_DECL_OVERRIDE;

protected:
  void changeEvent(QEvent *e) Q_DECL_OVERRIDE;

private:
  Ui::EditorTabImage *ui;
  QWidget *mEditorWidget;
  ImageEditor::Editor *mEditorObject;
  Data::Containers::ImageDocument *mDocument;
  AppUI::Status::StatusData *mStatusData;

  void initStatusData();
  void updateStatus();
  void updateSelectedImage();

private slots:
  void mon_documentChanged();
  void mon_editor_imageChanged(const QImage *value);
  void mon_editor_mouseMove(const QPoint *point);
  void mon_editor_scaleChanged(int scale);
signals:
  void documentChanged();
  void statusChanged();
};

} // namespace Images
} // namespace AppUI

#endif // EDITORTABIMAGE_H
