/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogSaveChanges;
}
//-----------------------------------------------------------------------------
class DialogSaveChanges : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveChanges(QWidget *parent = 0);
    ~DialogSaveChanges();
    enum SaveChangesResultCode
    {
        Save = 1,
        SaveAs = 2,
        DontSave = 3,
        Cancel = 0
    };
    int answer();

private:
    Ui::DialogSaveChanges *ui;
    SaveChangesResultCode mAnswer;
private slots:
    void on_buttonSave_clicked();
    void on_buttonSaveAs_clicked();
    void on_buttonDontSave_clicked();
    void on_buttonCancel_clicked();
};
//-----------------------------------------------------------------------------
#endif // DIALOGSAVECHANGES_H
