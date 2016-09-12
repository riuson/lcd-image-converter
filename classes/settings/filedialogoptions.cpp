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
int FileDialogOptions::openDocument_filterIndex()
{
    return FileDialogOptions::getInteger("openDocument_filterIndex");
}
//-----------------------------------------------------------------------------
void FileDialogOptions::setOpenDocument_filterIndex(int value)
{
    FileDialogOptions::setInteger("openDocument_filterIndex", value);
}
//-----------------------------------------------------------------------------
int FileDialogOptions::convertDocument_filterIndex()
{
    return FileDialogOptions::getInteger("convertDocument_filterIndex");
}
//-----------------------------------------------------------------------------
void FileDialogOptions::setConvertDocument_filterIndex(int value)
{
    FileDialogOptions::setInteger("convertDocument_filterIndex", value);
}
//-----------------------------------------------------------------------------
int FileDialogOptions::exportImage_filterIndex()
{
    return FileDialogOptions::getInteger("exportImage_filterIndex");
}
//-----------------------------------------------------------------------------
void FileDialogOptions::setExportImage_filterIndex(int value)
{
    FileDialogOptions::setInteger("exportImage_filterIndex", value);
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
