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

#include <QMainWindow>

#include "imainwindow.h"

namespace Ui
{
class MainWindow;
}

#include <QTranslator>
class WidgetBitmapEditor;

namespace Settings
{
class RecentList;
}

namespace AppUI
{
namespace Status
{
class StatusManager;
}

namespace MenuHandlers
{
class ActionFileHandlers;
class ActionEditHandlers;
class ActionImageHandlers;
class ActionFontHandlers;
class ActionSetupHandlers;
class ActionHelpHandlers;
} // namespace MenuHandlers

class MainWindow : public QMainWindow, public IMainWindow
{
  Q_OBJECT
  Q_INTERFACES(AppUI::IMainWindow)

public:
  MainWindow(QWidget* parent = 0);
  virtual ~MainWindow();

protected:
  void changeEvent(QEvent* e) Q_DECL_OVERRIDE;

private:
  Ui::MainWindow* ui;
  QTranslator* mTrans;
  Settings::RecentList* mRecentList;
  Status::StatusManager* mStatusManager;

  void selectLocale(const QString& localeName);
  void checkStartPageVisible();
  void createHandlers();
  void tabTextUpdate(QWidget* widget);
  int editorsCount() const;

  MenuHandlers::ActionFileHandlers* mFileHandlers;
  MenuHandlers::ActionEditHandlers* mEditHandlers;
  MenuHandlers::ActionImageHandlers* mImageHandlers;
  MenuHandlers::ActionFontHandlers* mFontHandlers;
  MenuHandlers::ActionSetupHandlers* mSetupHandlers;
  MenuHandlers::ActionHelpHandlers* mHelpHandlers;

private slots:
  void on_tabWidget_tabCloseRequested(int index);
  void on_tabWidget_currentChanged(int index);
  void setCurrentTab(QWidget* widget);
  void actionLanguage_triggered();

  void updateMenuState();
  void updateRecentList();
  void openRecentFile();

  void rememberFilename(const QString& filename);
  void closeRequest(QWidget* tab);
  void tabChanged(QWidget* tab);
  int tabCreated(QWidget* newTab);
  void statusChanged();

public:
  IEditor* currentEditor() Q_DECL_OVERRIDE;
  QWidget* currentTab() Q_DECL_OVERRIDE;
  void tabsList(QList<QWidget*>* list) Q_DECL_OVERRIDE;
  QWidget* parentWidget() Q_DECL_OVERRIDE;
  QString findAvailableName(const QString& prefix) Q_DECL_OVERRIDE;
};

} // namespace AppUI

#endif // MAINWINDOW_H
