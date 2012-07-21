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
#include "imainwindow.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class MainWindow;
}

class WidgetBitmapEditor;
class QTranslator;
class RecentList;
class ActionImageHandlers;
class ActionFontHandlers;
class ActionSetupHandlers;
class ActionHelpHandlers;
//-----------------------------------------------------------------------------
class MainWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT
    Q_INTERFACES(IMainWindow)

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
    RecentList *mRecentList;
    void updateMenuState();
    void selectLocale(const QString &localeName);
    int appendTab(QWidget *newTab, const QString &name);
    void checkStartPageVisible();

    ActionImageHandlers *mImageHandlers;
    ActionFontHandlers *mFontHandlers;
    ActionSetupHandlers *mSetupHandlers;
    ActionHelpHandlers *mHelpHandlers;

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
    void actionLanguage_triggered();

    void mon_editor_dataChanged();
    void updateRecentList();
    void openRecentFile();
    void openFile(const QString &filename);

public:
    IDocument *currentDocument();
    QWidget *currentTab();
    QWidget *parentWidget();
};
//-----------------------------------------------------------------------------
#endif // MAINWINDOW_H
