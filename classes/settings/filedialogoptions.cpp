/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#include "filedialogoptions.h"

#include <QFile>
#include <QDir>
#include <QMap>
#include <appsettings.h>

int FileDialogOptions::filterIndex(FileDialogOptions::Dialogs dialog)
{
  QString dialogName;

  if (FileDialogOptions::itemName(dialog, &dialogName)) {
    return FileDialogOptions::getInteger(QString("%1_filterIndex").arg(dialogName));
  }

  return 0;
}

void FileDialogOptions::setFilterIndex(FileDialogOptions::Dialogs dialog, int value)
{
  QString dialogName;

  if (FileDialogOptions::itemName(dialog, &dialogName)) {
    return FileDialogOptions::setInteger(QString("%1_filterIndex").arg(dialogName), value);
  }
}

const QString FileDialogOptions::directory(FileDialogOptions::Dialogs dialog)
{
  QString dialogName;

  if (FileDialogOptions::itemName(dialog, &dialogName)) {
    QString result = FileDialogOptions::getString(QString("%1_directory").arg(dialogName));

    if (!result.isEmpty()) {
      QDir dir(result);

      if (dir.exists()) {
        return dir.absolutePath();
      }
    }
  }

  return QDir::homePath();
}

void FileDialogOptions::setDirectory(FileDialogOptions::Dialogs dialog, const QString &value)
{
  QString dialogName;

  if (FileDialogOptions::itemName(dialog, &dialogName)) {
    return FileDialogOptions::setString(QString("%1_directory").arg(dialogName), value);
  }
}

bool FileDialogOptions::itemName(FileDialogOptions::Dialogs item, QString *name)
{
  QMap<Dialogs, QString> names;
  names[Dialogs::OpenDocument] = "openDocument";
  names[Dialogs::SaveDocument] =  "saveDocument";
  names[Dialogs::ConvertDocument] = "convertDocument";
  names[Dialogs::ImportImage] =  "importImage";
  names[Dialogs::ExportImage] =  "exportImage";
  names[Dialogs::ImportPreset] =  "importPreset";
  names[Dialogs::ExportPreset] =  "exportPreset";
  names[Dialogs::TemplateImage] =  "templateImage";
  names[Dialogs::TemplateFont] =  "templateFont";

  if (!names.contains(item)) {
    *name = QString();
    return false;
  }

  *name = names[item];
  return true;
}

int FileDialogOptions::getInteger(const QString &name)
{
  bool ok;

  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("filedialogs");
  int result = sett.value(name, QVariant(0)).toInt(&ok);
  sett.endGroup();

  if (!ok) {
    result = 0;
  }

  return result;
}

void FileDialogOptions::setInteger(const QString &name, int value)
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("filedialogs");
  sett.setValue(name, QVariant(value));
  sett.endGroup();
}

const QString FileDialogOptions::getString(const QString &name)
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("filedialogs");
  QString result = sett.value(name, QVariant(0)).toString();
  sett.endGroup();

  return result;
}

void FileDialogOptions::setString(const QString &name, const QString &value)
{
  AppSettings appsett;
  QSettings &sett = appsett.get();
  sett.beginGroup("filedialogs");
  sett.setValue(name, QVariant(value));
  sett.endGroup();
}

