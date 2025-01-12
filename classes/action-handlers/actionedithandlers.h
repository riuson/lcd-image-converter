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

#ifndef ACTIONEDITHANDLERS_H
#define ACTIONEDITHANDLERS_H

#include <QObject>

#include "actionhandlersbase.h"

namespace AppUI
{
namespace MenuHandlers
{

class ActionEditHandlers : public ActionHandlersBase
{
  Q_OBJECT
public:
  explicit ActionEditHandlers(QObject* parent = 0);
  virtual ~ActionEditHandlers() {}

public slots:
  void undo_triggered();
  void redo_triggered();
  void copy_triggered();
  void paste_triggered();
};

} // namespace MenuHandlers
} // namespace AppUI

#endif // ACTIONEDITHANDLERS_H
