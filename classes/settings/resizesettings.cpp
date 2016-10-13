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

#include "resizesettings.h"

#include <QVariant>
#include <QSettings>

int ResizeSettings::scale()
{
    QSettings sett;
    sett.beginGroup("crop-preview");
    bool ok;
    int result = sett.value("scale", QVariant(1)).toInt(&ok);
    sett.endGroup();

    if (ok)
        return result;
    return 1;
}

void ResizeSettings::setScale(int value)
{
    QSettings sett;
    sett.beginGroup("crop-preview");
    sett.setValue("scale", QVariant(value));
    sett.endGroup();
}

