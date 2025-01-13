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

#include "actionhelphandlers.h"

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

#include "dialogabout.h"
#include "dialogupdates.h"
#include "imainwindow.h"

namespace AppUI
{
namespace MenuHandlers
{

ActionHelpHandlers::ActionHelpHandlers(QObject* parent) : ActionHandlersBase(parent) {}

void ActionHelpHandlers::about_application_triggered()
{
  About::DialogAbout dialog(this->mMainWindow->parentWidget());
  dialog.exec();
}

void ActionHelpHandlers::about_qt_triggered() { QMessageBox::aboutQt(this->mMainWindow->parentWidget()); }

void ActionHelpHandlers::updates_triggered()
{
  AppUI::Updates::DialogUpdates dialog(this->mMainWindow->parentWidget());
  dialog.exec();
}

void ActionHelpHandlers::homepage_triggered()
{
  QDesktopServices::openUrl(QUrl("http://www.riuson.com/lcd-image-converter", QUrl::TolerantMode));
}

} // namespace MenuHandlers
} // namespace AppUI
