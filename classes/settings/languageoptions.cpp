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

#include "languageoptions.h"

#include <QFile>
#include <appsettings.h>

const QString LanguageOptions::locale()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("language");
  QString result = sett.value("selected", QVariant("")).toString();
  sett.endGroup();

  return result;
}

bool LanguageOptions::setLocale(const QString &value)
{
  bool isSuccessfully = false;

  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("language");
  QFile file(":/translations/" + value);

  if (file.exists()) {
    sett.setValue("selected", QVariant(value));
    isSuccessfully = true;
  } else {
    sett.setValue("selected", QVariant(""));
  }

  sett.endGroup();

  return isSuccessfully;
}

