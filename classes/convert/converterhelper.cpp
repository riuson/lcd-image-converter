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

#include "converterhelper.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QRegExp>
#include <QVector>
#include "bitstream.h"
#include "bitmaphelper.h"
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
void ConverterHelper::pixelsData(Preset *preset, QImage *image, QVector<quint32> *data, int *width, int *height)
{
    if (image != NULL && data != NULL && width != NULL && height != NULL)
    {
        data->clear();

        QImage im = *image;

        *width = im.width();
        *height = im.height();

        // monochrome image needs special preprocessing
        ConversionType type = preset->prepare()->convType();
        if (type == ConversionTypeMonochrome)
        {
            MonochromeType monotype = preset->prepare()->monoType();
            int edge = preset->prepare()->edge();

            switch (monotype)
            {
            case MonochromeTypeEdge:
                ConverterHelper::makeMonochrome(im, edge);
                break;
            case MonochromeTypeDiffuseDither:
                im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
                break;
            case MonochromeTypeOrderedDither:
                im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
                break;
            case MonochromeTypeThresholdDither:
                im = image->convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
                break;
            }
        }
        else if (type == ConversionTypeGrayscale)
        {
            ConverterHelper::makeGrayscale(im);
        }

        for (int y = 0; y < im.height(); y++)
        {
            for (int x = 0; x < im.width(); x++)
            {
                // typedef QRgb
                // An ARGB quadruplet on the format #AARRGGBB, equivalent to an unsigned int.
                QRgb pixel = im.pixel(x, y);
                quint32 value = pixel & 0x00ffffff;
                data->append(value);
            }
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::processPixels(Preset *preset, QVector<quint32> *data)
{
    if (preset != NULL && data != NULL)
    {
        for (int i = 0; i < data->size(); i++)
        {
            quint32 value = data->at(i);
            quint32 valueNew = 0;
            for (int j = 0; j < preset->matrix()->operationsCount(); j++)
            {
                quint32 mask;
                int shift;
                bool left;
                preset->matrix()->operation(j, &mask, &shift, &left);

                if (left)
                    valueNew |= (value & mask) << shift;
                else
                    valueNew |= (value & mask) >> shift;
            }

            if (preset->matrix()->operationsCount() == 0)
                valueNew = value;

            valueNew &= preset->matrix()->maskAnd();
            valueNew |= preset->matrix()->maskOr();
            data->replace(i, valueNew);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::packData(Preset *preset, QVector<quint32> *inputData, int inputWidth, int inputHeight, QVector<quint32> *outputData, int *outputWidth, int *outputHeight)
{
    *outputHeight = inputHeight;
    outputData->clear();

    int resultWidth = 0;
    int rowLength = 0;

    // each row
    for (int y = 0; y < inputHeight; y++)
    {
        // start of row in inputData
        int start = y * inputWidth;
        // get row data packed
        ConverterHelper::packDataRow(preset, inputData, start, inputWidth, outputData, &rowLength);
        // get row blocks count
        resultWidth = qMax(resultWidth, rowLength);
    }
    *outputWidth = resultWidth;
}
//-----------------------------------------------------------------------------
void ConverterHelper::prepareImage(Preset *preset, QImage *source, QImage *result)
{
    if (source != NULL)
    {
        QImage im = *source;

        switch (preset->prepare()->rotate())
        {
        case Rotate90:
            im = BitmapHelper::rotate90(source);
            break;
        case Rotate180:
            im = BitmapHelper::rotate180(source);
            break;
        case Rotate270:
            im = BitmapHelper::rotate270(source);
            break;
        case RotateNone:
        default:
            break;
        }
        if (preset->prepare()->flipHorizontal())
            im = BitmapHelper::flipHorizontal(&im);
        if (preset->prepare()->flipVertical())
            im = BitmapHelper::flipVertical(&im);
        if (preset->prepare()->inverse())
            im.invertPixels();

        *result = im;
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::createImagePreview(Preset *preset, QImage *source, QImage *result)
{
    if (source != NULL)
    {
        QImage im = *source;

        // simple prepare options
        switch (preset->prepare()->rotate())
        {
        case Rotate90:
            im = BitmapHelper::rotate90(source);
            break;
        case Rotate180:
            im = BitmapHelper::rotate180(source);
            break;
        case Rotate270:
            im = BitmapHelper::rotate270(source);
            break;
        case RotateNone:
        default:
            break;
        }
        if (preset->prepare()->flipHorizontal())
            im = BitmapHelper::flipHorizontal(&im);
        if (preset->prepare()->flipVertical())
            im = BitmapHelper::flipVertical(&im);
        if (preset->prepare()->inverse())
            im.invertPixels();

        // convert to mono/gray/color
        if (preset->prepare()->convType() == ConversionTypeMonochrome)
        {
            switch (preset->prepare()->monoType())
            {
            case MonochromeTypeEdge:
                ConverterHelper::makeMonochrome(im, preset->prepare()->edge());
                break;
            case MonochromeTypeDiffuseDither:
                im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
                break;
            case MonochromeTypeOrderedDither:
                im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
                break;
            case MonochromeTypeThresholdDither:
                im = im.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
                break;
            }
        }
        else if (preset->prepare()->convType() == ConversionTypeGrayscale)
        {
            ConverterHelper::makeGrayscale(im);
        }

        // mask used source data bits
        {
            // create mask
            quint32 mask = 0;
            switch (preset->prepare()->convType())
            {
            case ConversionTypeMonochrome:
            {
                quint32 opMask;
                int opShift;
                bool opLeft;
                for (int i = 0; i < preset->matrix()->operationsCount(); i++)
                {
                    preset->matrix()->operation(i, &opMask, &opShift, &opLeft);
                    if (opMask != 0)
                    {
                        mask = 0xffffffff;
                        break;
                    }
                }
                if (preset->matrix()->operationsCount() == 0)
                    mask = 0xffffffff;
                break;
            }
            case ConversionTypeGrayscale:
            {
                quint32 opMask;
                int opShift;
                bool opLeft;
                for (int i = 0; i < preset->matrix()->operationsCount(); i++)
                {
                    preset->matrix()->operation(i, &opMask, &opShift, &opLeft);
                    quint8 byte1 = (opMask >> 0) & 0xff;
                    quint8 byte2 = (opMask >> 8) & 0xff;
                    quint8 byte3 = (opMask >> 16) & 0xff;
                    quint8 byte4 = (opMask >> 24) & 0xff;
                    quint32 all = byte1 | byte2 | byte3 | byte4;
                    mask |= all;
                    mask |= all << 8;
                    mask |= all << 16;
                    mask |= all << 24;
                }
                if (preset->matrix()->operationsCount() == 0)
                    mask = 0xffffffff;
                break;
            }
            case ConversionTypeColor:
            {
                quint32 opMask;
                int opShift;
                bool opLeft;
                for (int i = 0; i < preset->matrix()->operationsCount(); i++)
                {
                    preset->matrix()->operation(i, &opMask, &opShift, &opLeft);
                    mask |= opMask;
                }
                if (preset->matrix()->operationsCount() == 0)
                    mask = 0xffffffff;
                break;
            }
            }

            // apply mask
            QPainter painter(&im);
            painter.setRenderHint(QPainter::Antialiasing, false);
            for (int x = 0; x < im.width(); x++)
            {
                for (int y = 0; y < im.height(); y++)
                {
                    QRgb value = im.pixel(x, y);
                    value &= mask;
                    QColor color = QColor(value);
                    painter.setPen(color);
                    painter.drawPoint(x, y);
                }
            }
        }

        *result = im;
    }
}
//-----------------------------------------------------------------------------
QString ConverterHelper::dataToString(Preset *preset, QVector<quint32> *data, int width, int height, const QString &prefix)
{
    QString result;
    DataBlockSize blockSize = preset->image()->blockSize();
    QChar temp[11];
    const QChar table[] = {
        QChar('0'), QChar('1'), QChar('2'), QChar('3'),
        QChar('4'), QChar('5'), QChar('6'), QChar('7'),
        QChar('8'), QChar('9'), QChar('a'), QChar('b'),
        QChar('c'), QChar('d'), QChar('e'), QChar('f') };
    const QChar comma = QChar(',');
    const QChar space = QChar(' ');
    const QChar end = QChar('\0');

    for (int y = 0; y < height; y++)
    {
        if (y > 0)
            result.append("\n");
        for (int x = 0; x < width; x++)
        {
            quint32 value = data->at(y * width + x);
            switch (blockSize)
            {
            case Data8:
                temp[0] = table[(value >> 4) & 0x0000000f];
                temp[1] = table[(value >> 0) & 0x0000000f];
                temp[2] = comma;
                temp[3] = space;
                temp[4] = end;
                break;
            case Data16:
                temp[0] = table[(value >> 12) & 0x0000000f];
                temp[1] = table[(value >> 8) & 0x0000000f];
                temp[2] = table[(value >> 4) & 0x0000000f];
                temp[3] = table[(value >> 0) & 0x0000000f];
                temp[4] = comma;
                temp[5] = space;
                temp[6] = end;
                break;
            case Data24:
                temp[0] = table[(value >> 20) & 0x0000000f];
                temp[1] = table[(value >> 16) & 0x0000000f];
                temp[2] = table[(value >> 12) & 0x0000000f];
                temp[3] = table[(value >> 8) & 0x0000000f];
                temp[4] = table[(value >> 4) & 0x0000000f];
                temp[5] = table[(value >> 0) & 0x0000000f];
                temp[6] = comma;
                temp[7] = space;
                temp[8] = end;
                break;
            case Data32:
                temp[0] = table[(value >> 28) & 0x0000000f];
                temp[1] = table[(value >> 24) & 0x0000000f];
                temp[2] = table[(value >> 20) & 0x0000000f];
                temp[3] = table[(value >> 16) & 0x0000000f];
                temp[4] = table[(value >> 12) & 0x0000000f];
                temp[5] = table[(value >> 8) & 0x0000000f];
                temp[6] = table[(value >> 4) & 0x0000000f];
                temp[7] = table[(value >> 0) & 0x0000000f];
                temp[8] = comma;
                temp[9] = space;
                temp[10] = end;
                break;
            }

            result += prefix + QString(temp);
        }
    }

    result.truncate(result.length() - 2);

    return result;
}
//-----------------------------------------------------------------------------
void ConverterHelper::makeMonochrome(QImage &image, int edge)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    edge &= 0x000000ff;
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            if (qGray(value) < edge)
                painter.setPen(QColor(0, 0, 0));
            else
                painter.setPen(QColor(255, 255, 255));
            painter.drawPoint(x, y);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::makeGrayscale(QImage &image)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            int gray = qGray(value);
            QColor color = QColor(gray ,gray, gray);
            painter.setPen(color);
            painter.drawPoint(x, y);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::packDataRow(Preset *preset, QVector<quint32> *inputData, int start, int count, QVector<quint32> *outputData, int *rowLength)
{
    *rowLength = 0;
    if (preset != NULL && inputData != NULL && outputData != NULL)
    {
        BitStream stream(preset, inputData, start, count);
        while (!stream.eof())
        {
            quint32 value = stream.next();

            if (preset->image()->bytesOrder() == BytesOrderBigEndian)
                value = ConverterHelper::toBigEndian(preset, value);

            outputData->append(value);
            (*rowLength)++;
        }
    }
}
//-----------------------------------------------------------------------------
quint32 ConverterHelper::toBigEndian(Preset *preset, quint32 value)
{
    quint8 src1, src2, src3, src4;
    src1 = value & 0xff;
    src2 = (value >> 8) & 0xff;
    src3 = (value >> 16) & 0xff;
    src4 = (value >> 24) & 0xff;

    quint32 result = 0;

    switch (preset->image()->blockSize())
    {
    case Data32:
        result |= src1 << 24;
        result |= src2 << 16;
        result |= src3 << 8;
        result |= src4;
        break;
    case Data24:
        result |= src1 << 16;
        result |= src2 << 8;
        result |= src3;
        break;
    case Data16:
        result |= src1 << 8;
        result |= src2;
        break;
    case Data8:
        result = src1;
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
