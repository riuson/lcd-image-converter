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
class ActionFileHandlers;
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
    WidgetBitmapEditor *mEditor;
    QTranslator *mTrans;
    RecentList *mRecentList;
    void updateMenuState();
    void selectLocale(const QString &localeName);
    void checkStartPageVisible();
    void createHandlers();

    ActionFileHandlers *mFileHandlers;
    ActionImageHandlers *mImageHandlers;
    ActionFontHandlers *mFontHandlers;
    ActionSetupHandlers *mSetupHandlers;
    ActionHelpHandlers *mHelpHandlers;

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void actionLanguage_triggered();

    void updateRecentList();
    void openRecentFile();

    void rememberFilename(const QString &filename);
    void closeRequest(QWidget *tab);
    void tabChanged(QWidget *tab, const QString &text, const QString &tooltip);
    int tabCreated(QWidget *newTab, const QString &name, const QString &tooltip);
public:
    IDocument *currentDocument();
    QWidget *currentTab();
    void tabsList(QList<QWidget *> *list);
    QWidget *parentWidget();
    QString findAvailableName(const QString &prefix);
};
//-----------------------------------------------------------------------------
#endif // MAINWINDOW_H
