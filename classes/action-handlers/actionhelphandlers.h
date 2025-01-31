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

#ifndef ACTIONHELPHANDLERS_H
#define ACTIONHELPHANDLERS_H

#include <QObject>

#include "actionhandlersbase.h"

namespace AppUI
{
namespace MenuHandlers
{

class ActionHelpHandlers : public ActionHandlersBase
{
  Q_OBJECT
public:
  explicit ActionHelpHandlers(QObject* parent = 0);
  virtual ~ActionHelpHandlers() {}

signals:

public slots:
  void about_application_triggered();
  void about_qt_triggered();
  void updates_triggered();
  void homepage_triggered();
};

} // namespace MenuHandlers
} // namespace AppUI

#endif // ACTIONHELPHANDLERS_H
