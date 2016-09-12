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

#ifndef FILEDIALOGOPTIONS_H
#define FILEDIALOGOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
//-----------------------------------------------------------------------------
class FileDialogOptions : public QObject
{
    Q_OBJECT
public:
    enum class Dialogs
    {
        None = 0,
        OpenDocument = 1,
        ConvertDocument = 2,
        ExportImage = 3
    };

    static int filterIndex(Dialogs dialog);
    static void setFilterIndex(Dialogs dialog, int value);

private:
    static bool itemName(Dialogs item, QString *name);
    static int getInteger(const QString &name);
    static void setInteger(const QString &name, int value);
};
//-----------------------------------------------------------------------------
#endif // FILEDIALOGOPTIONS_H
