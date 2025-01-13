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

#ifndef WIDGETBITMAPEDITOR_H
#define WIDGETBITMAPEDITOR_H

#include <QWidget>

namespace Ui
{
class WidgetBitmapEditor;
}

namespace AppUI
{
namespace Images
{

class WidgetBitmapEditor : public QWidget
{
  Q_OBJECT

public:
  explicit WidgetBitmapEditor(QWidget* parent = 0);
  virtual ~WidgetBitmapEditor();

  const QImage* image() const;
  void setImage(const QImage* value);

  QColor color1() const;
  QColor color2() const;
  int scale() const;

protected:
  void changeEvent(QEvent* e) Q_DECL_OVERRIDE;
  bool eventFilter(QObject* obj, QEvent* event) Q_DECL_OVERRIDE;
  void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;

private:
  Ui::WidgetBitmapEditor* ui;
  QImage mImageOriginal;
  QImage mImageScaled;
  QPixmap mPixmapScaled;
  int mScale;
  QColor mColor1;
  QColor mColor2;
  QPixmap mPixmapColor1;
  QPixmap mPixmapColor2;
  bool mFlagChanged;

  void updateImageScaled(int scale);
  void drawPixel(int x, int y, const QColor& color);
private slots:
  void on_spinBoxScale_valueChanged(int value);
  void on_pushButtonColor1_clicked();
  void on_pushButtonColor2_clicked();

public slots:
  void setScale(int value);
  void setColor1(const QColor value);
  void setColor2(const QColor value);

signals:
  void imageChanged();
  void mouseMove(QPoint point);
  void scaleSchanged(int scale);
};

} // namespace Images
} // namespace AppUI

#endif // WIDGETBITMAPEDITOR_H
