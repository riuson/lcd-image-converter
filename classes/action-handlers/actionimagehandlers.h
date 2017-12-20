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

#ifndef ACTIONIMAGEHANDLERS_H
#define ACTIONIMAGEHANDLERS_H

#include <QObject>
#include <QProcess>
#include "actionhandlersbase.h"

namespace AppUI
{
namespace MenuHandlers
{

class ActionImageHandlers : public ActionHandlersBase
{
  Q_OBJECT
public:
  explicit ActionImageHandlers(QObject *parent = 0);
  virtual ~ActionImageHandlers() {}

private:
  bool mRunningError;

signals:

public slots:
  void flipHorizontal_triggered();
  void flipVertical_triggered();
  void rotate_90_Clockwise_triggered();
  void rotate_180_triggered();
  void rotate_90_Counter_Clockwise_triggered();
  void shift_left_triggered();
  void shift_right_triggered();
  void shift_up_triggered();
  void shift_down_triggered();
  void inverse_triggered();
  void resize_triggered();
  void grayscale_triggered();
  void import_triggered();
  void export_triggered();
  void edit_in_external_tool_triggered();
};

} // namespace MenuHandlers
} // namespace AppUI

#endif // ACTIONIMAGEHANDLERS_H
