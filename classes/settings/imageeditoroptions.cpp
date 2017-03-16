/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#include "imageeditoroptions.h"

#include <QFile>
#include <appsettings.h>

QByteArray ImageEditorOptions::toolbarsState()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("toolbar-options");

  QByteArray result = sett.value("state", QByteArray()).toByteArray();

  sett.endGroup();
  sett.endGroup();

  return result;
}

void ImageEditorOptions::setToolbarsState(const QByteArray &value)
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("window-image-editor");
  sett.beginGroup("toolbar-options");

  sett.setValue("state", value);

  sett.endGroup();
  sett.endGroup();
}

