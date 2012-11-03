/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "bitmapeditoroptions.h"

#include <QSettings>
#include <QFile>
//-----------------------------------------------------------------------------
int BitmapEditorOptions::scale()
{
    QSettings sett;
    sett.beginGroup("setup");
    bool ok;
    int result = sett.value("defaultScale", QVariant(10)).toInt(&ok);
    sett.endGroup();

    return result;
}
//-----------------------------------------------------------------------------
void BitmapEditorOptions::setScale(int value)
{
    QSettings sett;
    sett.beginGroup("setup");
    sett.setValue("defaultScale", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
