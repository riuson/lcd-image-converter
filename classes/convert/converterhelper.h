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

#ifndef CONVERTERHELPER_H
#define CONVERTERHELPER_H
//-----------------------------------------------------------------------------
#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
class ConverterHelper
{
public:

    static void packDataPreview(
            QStringList *list,
            QStringList &colors,
            int bits,
            bool pack,
            bool alignToHigh);

    static void createMatrixMono(
            QList<quint32> *matrix,
            MonochromeType type = MonochromeTypeDiffuseDither,
            int edge = 0x80);
    static void createMatrixGrayscale(
            QList<quint32> *matrix,
            int bits = 8);
    static void createMatrixColor(
            QList<quint32> *matrix,
            int redBits = 8,
            int greenBits = 8,
            int blueBits = 8);

    // gets RGB array of pixels
    // also convert images to monochrome or grayscale (r = g = b = qGray(pixel))
    static void pixelsData(
            QList<quint32> *matrix,
            QImage *image,
            QList<quint32> *data,
            int *width,
            int *height);
    // apply masks and bits reorder
    static void processPixels(QList<quint32> *matrix, QList<quint32> *data);
    // pack data
    static void packData(
            QList<quint32> *matrix,
            QList<quint32> *inputData,
            int inputWidth,
            int inputHeight,
            QList<quint32> *outputData,
            int *outputWidth,
            int *outputHeight);
private:
    static void makeMonochrome(QImage &image, int edge);
    // make r = g = b = qGray(pixel)
    static void makeGrayscale(QImage &image);
    static void packDataRow(
            QList<quint32> *matrix,
            QList<quint32> *inputData,
            int start,
            int count,
            QList<quint32> *outputData);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERHELPER_H
