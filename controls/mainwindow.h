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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//-----------------------------------------------------------------------------
#include <QMainWindow>
//-----------------------------------------------------------------------------
namespace Ui {
    class MainWindow;
}

class WidgetBitmapEditor;
class QTranslator;
//-----------------------------------------------------------------------------
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QString findAvailableName(const QString &prefix);
    WidgetBitmapEditor *mEditor;
    QTranslator *mTrans;
    void updateMenuState();
    void selectLocale(const QString &localeName);

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actionNew_Image_triggered();
    void on_actionNew_Font_triggered();
    void on_actionOpen_triggered();
    void on_actionRename_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionClose_triggered();
    void on_actionConvert_triggered();
    void on_actionQuit_triggered();
    void on_actionImageFlip_Horizontal_triggered();
    void on_actionImageFlip_Vertical_triggered();
    void on_actionImageRotate_90_Clockwise_triggered();
    void on_actionImageRotate_180_triggered();
    void on_actionImageRotate_90_Counter_Clockwise_triggered();
    void on_actionImageInverse_triggered();
    void on_actionImageResize_triggered();
    void on_actionImageImport_triggered();
    void on_actionImageExport_triggered();
    void on_actionFontChange_triggered();
    void on_actionFontInverse_triggered();
    void on_actionFontResize_triggered();
    void on_actionFontMinimizeHeight_triggered();
    void on_actionSetupConversion_triggered();
    void on_actionSetupTemplates_triggered();
    void on_actionAbout_triggered();
    void actionLanguage_triggered();

    void mon_editor_dataChanged();
};
//-----------------------------------------------------------------------------
#endif // MAINWINDOW_H
