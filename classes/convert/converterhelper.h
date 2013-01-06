/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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
#include "preset.h"
//-----------------------------------------------------------------------------
class QImage;
class Preset;
//-----------------------------------------------------------------------------
class ConverterHelper
{
public:
    // gets RGB array of pixels
    // also convert images to monochrome or grayscale (r = g = b = qGray(pixel))
    static void pixelsData(
            Preset *matrix,
            QImage *image,
            QVector<quint32> *data,
            int *width,
            int *height);
    // apply masks and bits reorder
    static void processPixels(Preset *matrix, QVector<quint32> *data);
    // pack data
    static void packData(
            Preset *matrix,
            QVector<quint32> *inputData,
            int inputWidth,
            int inputHeight,
            QVector<quint32> *outputData,
            int *outputWidth,
            int *outputHeight);
    // bits reordering
    static void reorder(
            Preset *preset,
            QVector<quint32> *inputData, int inputWidth, int inputHeight,
            QVector<quint32> *outputData, int *outputWidth, int *outputHeight);

    static void compressData(
            Preset *matrix,
            QVector<quint32> *inputData, int inputWidth, int inputHeight,
            QVector<quint32> *outputData, int *outputWidth, int *outputHeight);

    static void prepareImage(Preset *matrix, QImage *source, QImage *result);
    static void createImagePreview(Preset *matrix, QImage *source, QImage *result);

    static QString dataToString(
            Preset *matrix,
            QVector<quint32> *data, int width, int height,
            const QString &prefix);
private:
    static void makeMonochrome(QImage &image, int edge);
    // make r = g = b = qGray(pixel)
    static void makeGrayscale(QImage &image);
    static void packDataRow(
            Preset *matrix,
            QVector<quint32> *inputData,
            int start,
            int count,
            QVector<quint32> *outputData,
            int *rowLength);
    static quint32 toBigEndian(Preset *matrix, quint32 value);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERHELPER_H
