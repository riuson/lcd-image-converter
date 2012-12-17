/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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
#include "limits"
//-----------------------------------------------------------------------------
#include <QPainter>
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
QImage BitmapHelper::shiftUp(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.drawImage(0, -1, *source);
    painter.drawImage(0, source->height() - 1, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::shiftRight(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.drawImage(1, 0, *source);
    painter.drawImage(-source->width() + 1, 0, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::shiftDown(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.drawImage(0, 1, *source);
    painter.drawImage(0, -source->height() + 1, *source);
    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::shiftLeft(QImage *source)
{
    QImage result = QImage(source->width(), source->height(), source->format());
    QPainter painter(&result);
    painter.drawImage(-1, 0, *source);
    painter.drawImage(source->width() - 1, 0, *source);
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
QImage BitmapHelper::resize(QImage *source, int width, int height, int offsetX, int offsetY, bool center, bool changeWidth, bool changeHeight, const QColor &backColor)
{
    if (!changeWidth)
    {
        width = source->width();
        offsetX = 0;
    }
    if (!changeHeight)
    {
        height = source->height();
        offsetY = 0;
    }
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
void BitmapHelper::findEmptyArea(const QImage *source, int *left, int *top, int *right, int *bottom)
{
    QRgb background = source->pixel(0, 0);
    // max possible values by default
    int l = std::numeric_limits<int>::max();
    int t = std::numeric_limits<int>::max();
    int r = 0;
    int b = 0;

    // search from left/top to bottom/right
    for (int x = 0; x < source->width(); x++)
    {
        for (int y = 0; y < source->height(); y++)
        {
            if (source->pixel(x, y) != background)
            {
                l = qMin(l, x);
                t = qMin(t, y);
                r = qMax(r, x);
                b = qMax(b, y);
            }
        }
    }
    *left = l;
    *top = t;
    *right = r;
    *bottom = b;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::scale(const QImage *source, int scale)
{
    int width = source->width();
    int height = source->height();

    QImage result = source->scaled(width * scale, height * scale, Qt::KeepAspectRatio, Qt::FastTransformation);

    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::drawGrid(const QImage *source, int scale)
{
    QImage result = QImage(*source);

    if (scale > 5)
    {
        QPixmap pixmap = QPixmap::fromImage(result);

        QPainter painter(&pixmap);

        painter.setPen(QColor("silver"));

        for (int x = 0; x < pixmap.width(); x += scale)
        {
            painter.drawLine(x, 0, x, pixmap.height());
        }
        for (int y = 0; y < pixmap.height(); y += scale)
        {
            painter.drawLine(0, y, pixmap.width(), y);
        }

        result = pixmap.toImage();
    }

    return result;
}
//-----------------------------------------------------------------------------
QImage BitmapHelper::drawPixel(const QImage *source, int x, int y, const QColor &color)
{
    QPixmap pixmap = QPixmap::fromImage(*source);

    QPainter painter(&pixmap);
    painter.setPen(color);
    painter.drawPoint(x, y);

    return pixmap.toImage();
}
//-----------------------------------------------------------------------------
