/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
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

#ifndef DIALOGFONTRANGE_H
#define DIALOGFONTRANGE_H

#include <QCompleter>
#include <QDialog>

namespace Ui
{
class DialogFontRange;
}

namespace AppUI
{
namespace Fonts
{

class DialogFontRange : public QDialog
{
  Q_OBJECT

public:
  explicit DialogFontRange(QWidget* parent = 0);
  virtual ~DialogFontRange();

  const QString& resultString() const;

private:
  Ui::DialogFontRange* ui;
  QString mResultString;
  QCompleter* mEncodingCompleter;

private slots:
  void updatePreview();
  void updatePreview(const QString& encoding, int from, int to, bool bigEndian);
  void on_plainTextEditPreview_textChanged();
};

} // namespace Fonts
} // namespace AppUI

#endif // DIALOGFONTRANGE_H
