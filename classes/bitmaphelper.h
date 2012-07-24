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

#ifndef BITMAPHELPER_H
#define BITMAPHELPER_H
//-----------------------------------------------------------------------------
#include <QImage>
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
    enum BitmapHelperTransformCodes
    {
        None = 0,
        Rotate90 = 1,
        Rotate180 = 2,
        Rotate270 = 3,
        FlipHorizontal = 4,
        FlipVertical = 8,
        Inverse = 16
    };
    static QImage transform(BitmapHelperTransformCodes type, QImage *source);
    static QImage rotate90(QImage *source);
    static QImage rotate180(QImage *source);
    static QImage rotate270(QImage *source);
    static QImage flipHorizontal(QImage *source);
    static QImage flipVertical(QImage *source);
    static QImage resize(QImage *source, int width, int height, int offsetX, int offsetY, bool center, bool changeWidth, bool changeHeight, const QColor &backColor);
    static void findEmptyArea(const QImage *source, int *left, int *top, int *right, int *bottom);
    static QImage createImageScaled(QImage *original, int scale);
    static void drawGrid(QImage *original, QPixmap &pixmap, QPainter *painter, int scale);
};
//-----------------------------------------------------------------------------
#endif // BITMAPHELPER_H
