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

#ifndef DIALOGFONTCHANGED_H
#define DIALOGFONTCHANGED_H

#include <QDialog>

namespace Ui
{
class DialogFontChanged;
}

namespace AppUI
{
namespace Fonts
{

class DialogFontChanged : public QDialog
{
  Q_OBJECT

public:
  explicit DialogFontChanged(QWidget* parent = 0);
  virtual ~DialogFontChanged();

  bool regenerateAll() const;

private:
  Ui::DialogFontChanged* ui;

  bool mRegenerateAll;

private slots:
  void on_buttonRegenerateAll_clicked();
  void on_buttonGenerateNewOnly_clicked();
  void on_buttonCancel_clicked();
};

} // namespace Fonts
} // namespace AppUI

#endif // DIALOGFONTCHANGED_H
