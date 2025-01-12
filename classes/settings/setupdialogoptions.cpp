/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
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

#include "setupdialogoptions.h"

#include <QVariant>

#include "appsettings.h"

namespace Settings
{

int SetupDialogOptions::animationTotalTime()
{
  AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("setup-dialog");
  bool ok;
  int result = sett.value("animationTotalTime", QVariant(60)).toInt(&ok);
  sett.endGroup();

  if (!ok) {
    result = 60;
  }

  return result;
}

void SetupDialogOptions::setAnimationTime(int value)
{
  AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("setup-dialog");
  sett.setValue("animationTotalTime", QVariant(value));
  sett.endGroup();
}

int SetupDialogOptions::animationInterval()
{
  AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("setup-dialog");
  bool ok;
  int result = sett.value("animationInterval", QVariant(100)).toInt(&ok);
  sett.endGroup();

  if (!ok) {
    result = 100;
  }

  return result;
}

void SetupDialogOptions::setAnimationInterval(int value)
{
  AppSettings appsett;
  QSettings& sett = appsett.get();
  sett.beginGroup("setup-dialog");
  sett.setValue("animationInterval", QVariant(value));
  sett.endGroup();
}

} // namespace Settings
