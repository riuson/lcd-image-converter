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

#include "actionsetuphandlers.h"

#include "datacontainer.h"
#include "dialogexternaleditor.h"
#include "dialogoptions.h"
#include "idocument.h"
#include "ieditor.h"
#include "imainwindow.h"

namespace AppUI
{
namespace MenuHandlers
{

ActionSetupHandlers::ActionSetupHandlers(QObject* parent) : ActionHandlersBase(parent) {}

void ActionSetupHandlers::conversion_triggered()
{
  Data::Containers::DataContainer* data = nullptr;

  if (this->editor() != nullptr) {
    data = this->editor()->document()->dataContainer();
  }

  AppUI::Setup::DialogOptions dialog(data, this->mMainWindow->parentWidget());
  dialog.exec();
}

void ActionSetupHandlers::external_editor_triggered()
{
  AppUI::Setup::DialogExternalEditor dialog(this->mMainWindow->parentWidget());
  dialog.exec();
}

} // namespace MenuHandlers
} // namespace AppUI
