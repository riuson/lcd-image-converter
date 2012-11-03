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
    sett.beginGroup("bitmap-editor");
    bool ok;
    int result = sett.value("defaultScale", QVariant(10)).toInt(&ok);
    sett.endGroup();

    return result;
}
//-----------------------------------------------------------------------------
void BitmapEditorOptions::setScale(int value)
{
    QSettings sett;
    sett.beginGroup("bitmap-editor");
    sett.setValue("defaultScale", QVariant(value));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
QColor BitmapEditorOptions::color1()
{
    QSettings sett;
    sett.beginGroup("bitmap-editor");
    bool ok;
    unsigned int a = sett.value("color1", QVariant("none")).toUInt(&ok);
    sett.endGroup();

    QColor result = QColor("black");
    if (ok)
    {
        result = QColor(QRgb(a));
    }

    return result;
}
//-----------------------------------------------------------------------------
QColor BitmapEditorOptions::color2()
{
    QSettings sett;
    sett.beginGroup("bitmap-editor");
    bool ok;
    unsigned int a = sett.value("color2", QVariant("none")).toUInt(&ok);
    sett.endGroup();

    QColor result = QColor("white");
    if (ok)
    {
        result = QColor(QRgb(a));
    }

    return result;
}
//-----------------------------------------------------------------------------
void BitmapEditorOptions::setColor1(const QColor &color)
{
    unsigned int a = color.rgb();

    QSettings sett;
    sett.beginGroup("bitmap-editor");
    sett.setValue("color1", QVariant(a));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void BitmapEditorOptions::setColor2(const QColor &color)
{
    unsigned int a = color.rgb();

    QSettings sett;
    sett.beginGroup("bitmap-editor");
    sett.setValue("color2", QVariant(a));
    sett.endGroup();
}
//-----------------------------------------------------------------------------
