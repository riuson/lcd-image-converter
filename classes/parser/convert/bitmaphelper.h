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

#ifndef BITMAPHELPER_H
#define BITMAPHELPER_H
//-----------------------------------------------------------------------------
#include <QImage>
//-----------------------------------------------------------------------------
#include <conversion_options.h>
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
typedef struct
{
    int width;
    int height;
    int offsetX;
    int offsetY;
    bool center;
} tResizeInfo;
//-----------------------------------------------------------------------------
class BitmapHelper
{
public:
    static QImage rotate90(const QImage *source);
    static QImage rotate180(const QImage *source);
    static QImage rotate270(const QImage *source);
    static QImage shiftUp(const QImage *source);
    static QImage shiftRight(const QImage *source);
    static QImage shiftDown(const QImage *source);
    static QImage shiftLeft(const QImage *source);
    static QImage flipHorizontal(const QImage *source);
    static QImage flipVertical(const QImage *source);
    static QImage resize(const QImage *source, int width, int height, int offsetX, int offsetY, bool center, bool changeWidth, bool changeHeight, const QColor &backColor);
    static QImage crop(const QImage *source, int left, int top, int right, int bottom, const QColor &backColor);
    static void findEmptyArea(const QImage *source, int *left, int *top, int *right, int *bottom);
    static QImage scale(const QImage *source, int scale);
    static QImage drawGrid(const QImage *source, int scale);
    static QImage drawPixel(const QImage *source, int x, int y, const QColor &color);
    static QColor detectBackgroundColor(const QImage *image);
    static QImage fromSvg(const QString &path, int size);
};
//-----------------------------------------------------------------------------
#endif // BITMAPHELPER_H
