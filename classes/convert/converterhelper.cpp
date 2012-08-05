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
//-----------------------------------------------------------------------------
void ConverterHelper::packDataPreview(QStringList *list, QStringList &colors, int bits, bool pack, bool alignToHigh)
{
    QStringList temp;
    for (int i = 0, j = 0, k = 0; i < 80; i++)
    {
        QString a = colors.at(k++);
        temp.append(a.replace(".", QString("%1.").arg(j)));
        if (k == colors.length())// end of point (color bits)
        {
            j++;
            k = 0;
            if (!pack)
            {
                if ((i % bits) + colors.length() > bits - 1)
                {
                    while (i % bits != bits - 1)
                    {
                        if (alignToHigh)
                            temp.append("0");
                        else
                            temp.prepend("0");
                        i++;
                    }
                    list->append(temp);
                    temp.clear();
                }
            }
            else
            {
                list->append(temp);
                temp.clear();
            }
        }
    }
    list->append(temp);
    temp.clear();
}
//-----------------------------------------------------------------------------
void ConverterHelper::createMatrixMono(QList<quint32> *matrix, MonochromeType type, int edge)
{
    if (matrix != NULL)
    {
        matrix->clear();

        ConversionMatrixOptions options(matrix);

        options.setConvType(ConversionTypeMonochrome);
        options.setMonoType(type);
        options.setEdge(edge);
        options.setBlockSize(Data8);
        options.setMask(0x00000001);

        // bits shift
        {
            matrix->append(0x00000001);

            matrix->append(0x00000000);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::createMatrixGrayscale(QList<quint32> *matrix, int bits)
{
    if (matrix != NULL)
    {
        matrix->clear();

        if (bits > 8) bits = 8;
        if (bits < 1) bits = 1;

        ConversionMatrixOptions options(matrix);

        options.setConvType(ConversionTypeGrayscale);
        options.setBlockSize(Data16);

        // mask of used bits before packing
        {
            quint32 mask = 0x000000ff;
            mask = mask << bits;
            mask = mask >> 8;
            mask = mask & 0x000000ff;

            options.setMask(0x00000001);
        }

        // bits shift
        {
            quint32 mask = 0x0000ff00;
            mask = mask >> bits;
            mask = mask & 0x000000ff;
            matrix->append(mask);

            quint32 shift = 8 - bits;
            matrix->append(shift);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::createMatrixColor(QList<quint32> *matrix, int redBits, int greenBits, int blueBits)
{
    if (matrix != NULL)
    {
        matrix->clear();

        if (redBits > 8) redBits = 8;
        if (redBits < 1) redBits = 1;
        if (greenBits > 8) greenBits = 8;
        if (greenBits < 1) greenBits = 1;
        if (blueBits > 8) blueBits = 8;
        if (blueBits < 1) blueBits = 1;

        int bits = redBits + greenBits + blueBits;

        ConversionMatrixOptions options(matrix);

        options.setConvType(ConversionTypeColor);
        options.setBlockSize(Data32);

        // mask of used bits before packing
        {
            quint64 mask64 = 0x00000000ffffffff;
            mask64 = mask64 << bits;
            mask64 = mask64 >> 32;
            mask64 = mask64 & 0x0000000000ffffff; // 24 bits
            quint32 mask = (quint32)mask64;
            options.setMask(mask);
        }

        // red bits shift
        {
            quint32 mask = 0x0000ff00;
            mask = mask >> redBits;
            mask = mask & 0x000000ff;
            mask = mask << 16;
            matrix->append(mask);

            quint32 shift = 24 - bits;
            matrix->append(shift);
        }

        // green bits shift
        {
            quint32 mask = 0x0000ff00;
            mask = mask >> greenBits;
            mask = mask & 0x000000ff;
            mask = mask << 8;
            matrix->append(mask);

            quint32 shift = 16 - greenBits - blueBits;
            matrix->append(shift);
        }

        // blue bits shift
        {
            quint32 mask = 0x0000ff00;
            mask = mask >> blueBits;
            mask = mask & 0x000000ff;
            matrix->append(mask);

            quint32 shift = 8 - blueBits;
            matrix->append(shift);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::pixelsData(QList<quint32> *matrix, QImage *image, QList<quint32> *data, int *width, int *height)
{
    if (image != NULL && data != NULL && width != NULL && height != NULL)
    {
        data->clear();

        ConversionMatrixOptions options(matrix);

        QImage im = *image;

        *width = im.width();
        *height = im.height();

        // monochrome image needs special preprocessing
        ConversionType type = options.convType();
        if (type == ConversionTypeMonochrome)
        {
            MonochromeType monotype = options.monoType();
            int edge = options.edge();

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
void ConverterHelper::processPixels(QList<quint32> *matrix, QList<quint32> *data)
{
    if (matrix != NULL && data != NULL)
    {
        for (int i = 0; i < data->length(); i++)
        {
            quint32 value = data->at(i);
            quint32 valueNew = 0;
            for (int j = 2; j < matrix->length(); j += 2)
            {
                quint32 mask = matrix->at(i);
                quint32 shift = matrix->at(i + 1);

                if (shift & 0x80000000)
                    valueNew |= (value & mask) << (shift & 0x0000001f);
                else
                    valueNew |= (value & mask) >> (shift & 0x0000001f);
            }
            valueNew &= matrix->at(1);
            data->replace(i, valueNew);
        }
    }
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
