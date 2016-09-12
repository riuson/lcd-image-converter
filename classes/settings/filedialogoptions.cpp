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

#include <QSettings>
#include <QFile>
//-----------------------------------------------------------------------------
int FileDialogOptions::filterIndex(FileDialogOptions::Dialogs dialog)
{
    QString dialogName;

    if (FileDialogOptions::itemName(dialog, &dialogName))
    {
        return FileDialogOptions::getInteger(QString("%1_filterIndex").arg(dialogName));
    }

    return 0;
}
//-----------------------------------------------------------------------------
void FileDialogOptions::setFilterIndex(FileDialogOptions::Dialogs dialog, int value)
{
    QString dialogName;

    if (FileDialogOptions::itemName(dialog, &dialogName))
    {
        return FileDialogOptions::setInteger(QString("%1_filterIndex").arg(dialogName), value);
    }
}
//-----------------------------------------------------------------------------
bool FileDialogOptions::itemName(FileDialogOptions::Dialogs item, QString *name)
{
    QStringList names;
    names << "none"
          << "openDocument"
          << "convertDocument"
          << "exportImage";

    int index = (int)item;

    if (index == 0 || index >= names.count())
    {
        *name = QString();
        return false;
    }

    *name = names.at(index);
    return true;
}
//-----------------------------------------------------------------------------
int FileDialogOptions::getInteger(const QString &name)
{
    bool ok;

    QSettings sett;
    sett.beginGroup("filedialogs");
    int result = sett.value(name, QVariant(0)).toInt(&ok);
    sett.endGroup();

    if (!ok) {
        result = 0;
    }

    return result;
}
//-----------------------------------------------------------------------------
void FileDialogOptions::setInteger(const QString &name, int value)
{
    QSettings sett;
    sett.beginGroup("filedialogs");
    sett.setValue(name, QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
