/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef DIALOGFONTPREVIEW_H
#define DIALOGFONTPREVIEW_H

#include <QDialog>
#include <QImage>
#include <QPixmap>

#include "idocument.h"

namespace Ui
{
class DialogFontPreview;
}

namespace AppUI
{
namespace Fonts
{

class DialogFontPreview : public QDialog
{
  Q_OBJECT

public:
  explicit DialogFontPreview(QWidget* parent = 0);
  virtual ~DialogFontPreview();

  void setDocument(Data::Containers::IDocument* document);

private:
  Ui::DialogFontPreview* ui;
  Data::Containers::IDocument* mDocument;
  QImage mOriginalImage;
  QImage mScaledImage;

private slots:
  void on_lineEditText_textChanged(const QString& text);
  void on_spinBoxScale_valueChanged(int i);
};

} // namespace Fonts
} // namespace AppUI

#endif // DIALOGFONTPREVIEW_H
