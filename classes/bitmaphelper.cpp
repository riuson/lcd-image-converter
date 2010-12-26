/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
#include <QPainter>
//-----------------------------------------------------------------------------
QImage BitmapHelper::transform(BitmapHelperTransformCodes type, QImage *source)
{
    QImage result(*source);
    switch (type)
    {
    case Rotate90:
        result = rotate90(source);
        break;
    case Rotate180:
        result = rotate180(source);
        break;
    case Rotate270:
        result = rotate270(source);
        break;
    case FlipHorizontal:
        result = flipHorizontal(source);
        break;
    case FlipVertical:
        result = flipVertical(source);
        break;
    case Inverse:
        result.invertPixels();
        break;
    default:
        break;
    }
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate90(QImage *source)
{
    QImage result = QImage(source->height(), source->width(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(result.width(), 0));
    painter.rotate(90);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate180(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(result.width(), result.height()));
    painter.rotate(180);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::rotate270(QImage *source)
{
    QImage result = QImage(source->height(), source->width(), source->format());
    QPainter painter(&result);
    painter.translate(QPoint(0, result.height()));
    painter.rotate(270);
    painter.drawImage(0, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::flipHorizontal(QImage *source)
{
    QImage result = source->mirrored(true, false);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::flipVertical(QImage *source)
{
    QImage result = source->mirrored(false, true);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::resize(QImage *source, int width, int height, int offsetX, int offsetY, bool center, const QColor &backColor)
{
    QImage result = QImage(width, height, source->format());
    QPainter painter(&result);
    painter.fillRect(0, 0, width, height, backColor);
    if (!center)
        painter.drawImage(offsetX, offsetY, *source);
    else
    {
        int centerX = width / 2;
        int centerY = height / 2;
        offsetX = centerX - source->width() / 2;
        offsetY = centerY - source->height() / 2;
        painter.drawImage(offsetX, offsetY, *source);
    }
    return result;
}
//-----------------------------------------------------------------------------
