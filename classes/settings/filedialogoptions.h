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

        OpenDocument,
        SaveDocument,
        ConvertDocument,

        ImportImage,
        ExportImage,

        ImportPreset,
        ExportPreset,

        TemplateImage,
        TemplateFont
    };

    static int filterIndex(Dialogs dialog);
    static void setFilterIndex(Dialogs dialog, int value);
    static const QString directory(Dialogs dialog);
    static void setDirectory(Dialogs dialog, const QString &value);

private:
    static bool itemName(Dialogs item, QString *name);
    static int getInteger(const QString &name);
    static void setInteger(const QString &name, int value);
    static const QString getString(const QString &name);
    static void setString(const QString &name, const QString &value);
};
//-----------------------------------------------------------------------------
#endif // FILEDIALOGOPTIONS_H
