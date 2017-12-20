/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "fonteditoroptions.h"
#include "bitmaphelper.h"
#include <QVariant>
#include <QRgb>
#include <appsettings.h>

namespace Settings
{

int FontEditorOptions::scale()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  bool ok;
  int result = sett.value("scale", QVariant(1)).toInt(&ok);
  sett.endGroup();

  if (ok) {
    return result;
  }

  return 1;
}

void FontEditorOptions::setScale(int value)
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  sett.setValue("scale", QVariant(value));
  sett.endGroup();
}

QColor FontEditorOptions::foreColor()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  bool ok;
  unsigned int a = sett.value("foreColor", QVariant("none")).toUInt(&ok);
  sett.endGroup();

  QColor result = QColor("black");

  if (ok) {
    result = Parsing::Conversion::BitmapHelper::fromRgba(QRgb(a));
  }

  return result;
}

QColor FontEditorOptions::backColor()
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  bool ok;
  unsigned int a = sett.value("backColor", QVariant("none")).toUInt(&ok);
  sett.endGroup();

  QColor result = QColor("white");

  if (ok) {
    result = Parsing::Conversion::BitmapHelper::fromRgba(QRgb(a));
  }

  return result;
}

void FontEditorOptions::setForeColor(const QColor &value)
{
  unsigned int a = value.rgba();

  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  sett.setValue("foreColor", QVariant(a));
  sett.endGroup();
}

void FontEditorOptions::setBackColor(const QColor &value)
{
  unsigned int a = value.rgba();

  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("font-editor");
  sett.setValue("backColor", QVariant(a));
  sett.endGroup();
}

} // namespace Settings
