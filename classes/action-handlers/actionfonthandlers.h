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

#ifndef ACTIONFONTHANDLERS_H
#define ACTIONFONTHANDLERS_H

#include <QObject>

#include "actionhandlersbase.h"

namespace AppUI
{
namespace MenuHandlers
{

class ActionFontHandlers : public ActionHandlersBase
{
  Q_OBJECT
public:
  explicit ActionFontHandlers(QObject *parent = 0);
  virtual ~ActionFontHandlers() {}

signals:
  void imageCreated(QImage *image, const QString &documentName);

public slots:
  void fontChange_triggered();
  void fontInverse_triggered();
  void fontResize_triggered();
  void fontPreview_triggered();
  void fontToImage_triggered();
};

} // namespace MenuHandlers
} // namespace AppUI

#endif // ACTIONFONTHANDLERS_H
