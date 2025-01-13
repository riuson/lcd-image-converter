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

#ifndef WINDOWEDITOR_H
#define WINDOWEDITOR_H

#include <QMainWindow>

namespace Ui
{
class WindowEditor;
}

namespace ImageEditor
{
namespace Tools
{
class ToolsManager;
class IImageEditorTool;
} // namespace Tools
} // namespace ImageEditor

namespace AppUI
{
namespace Images
{

class WindowEditor : public QMainWindow
{
  Q_OBJECT

public:
  explicit WindowEditor(QWidget* parent = 0);
  virtual ~WindowEditor();

  const QImage* image() const;
  void setImage(const QImage* value);

  int scale() const;

protected:
  void changeEvent(QEvent* e) Q_DECL_OVERRIDE;
  bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
  Ui::WindowEditor* ui;
  QImage mImageOriginal;
  QImage mImageScaled;
  QPixmap mPixmapScaled;
  ImageEditor::Tools::ToolsManager* mTools;
  ImageEditor::Tools::IImageEditorTool* mSelectedTool;

  void updateImageScaled(int value);
  void updateImageScaled(const QImage& image, int scale);
  void drawPixel(int x, int y, const QColor& color);
  void createTools();

private slots:
  void tool_started(const QImage* value);
  void tool_processing(const QImage* value);
  void tool_completed(const QImage* value, bool changed);
  void tool_scaleChanged(int value);
  void tool_selectionChanged(const QPainterPath& value);

public slots:
  void toolChanged(int toolIndex);

signals:
  void imageChanged();
  void mouseMove(const QPoint* point);
  void scaleChanged(int scale);
};

} // namespace Images
} // namespace AppUI

#endif // WINDOWEDITOR_H
