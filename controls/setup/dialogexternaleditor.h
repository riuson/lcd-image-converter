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

#ifndef DIALOGEXTERNALEDITOR_H
#define DIALOGEXTERNALEDITOR_H

#include <QDialog>

namespace Ui
{
class DialogExternalEditor;
}

namespace AppUI
{
namespace Setup
{

class DialogExternalEditor : public QDialog
{
  Q_OBJECT

public:
  explicit DialogExternalEditor(QWidget* parent = 0);
  virtual ~DialogExternalEditor();

private:
  Ui::DialogExternalEditor* ui;

private slots:
  void on_pushButtonBrowse_clicked();

protected:
  void done(int result) Q_DECL_OVERRIDE;
};

} // namespace Setup
} // namespace AppUI

#endif // DIALOGEXTERNALEDITOR_H
