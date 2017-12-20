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

#ifndef DIALOGSAVECHANGES_H
#define DIALOGSAVECHANGES_H

#include <QObject>

class QMessageBox;

namespace AppUI
{
namespace CommonDialogs
{

class DialogSaveChanges : public QObject
{
  Q_OBJECT

public:
  explicit DialogSaveChanges(const QString &documentName, QWidget *parent = 0);
  virtual ~DialogSaveChanges();

  enum SaveChangesResultCode {
    Save = 1,
    SaveAs = 2,
    DontSave = 3,
    Cancel = 0
  };
  int answer();
  int exec();

private:
  SaveChangesResultCode mAnswer;
  QMessageBox *mBox;

private slots:
  void on_messageBox_finished(int result);
};

} // namespace CommonDialogs
} // namespace AppUI

#endif // DIALOGSAVECHANGES_H
