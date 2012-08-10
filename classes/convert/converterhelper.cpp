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
#include "bitstream.h"
#include "bitmaphelper.h"
#include "conversionmatrix.h"
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
void ConverterHelper::pixelsData(ConversionMatrix *matrix, QImage *image, QList<quint32> *data, int *width, int *height)
{
    if (image != NULL && data != NULL && width != NULL && height != NULL)
    {
        data->clear();

        QImage im = *image;

        *width = im.width();
        *height = im.height();

        // monochrome image needs special preprocessing
        ConversionType type = matrix->options()->convType();
        if (type == ConversionTypeMonochrome)
        {
            MonochromeType monotype = matrix->options()->monoType();
            int edge = matrix->options()->edge();

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
void ConverterHelper::processPixels(ConversionMatrix *matrix, QList<quint32> *data)
{
    if (matrix != NULL && data != NULL)
    {
        for (int i = 0; i < data->length(); i++)
        {
            quint32 value = data->at(i);
            quint32 valueNew = 0;
            for (int j = 0; j < matrix->operationsCount(); j++)
            {
                quint32 mask;
                int shift;
                bool left;
                matrix->operation(j, &mask, &shift, &left);

                if (left)
                    valueNew |= (value & mask) << shift;
                else
                    valueNew |= (value & mask) >> shift;
            }
            valueNew &= matrix->options()->maskAnd();
            valueNew |= matrix->options()->maskOr();
            data->replace(i, valueNew);
        }
    }
}
//-----------------------------------------------------------------------------
void ConverterHelper::packData(ConversionMatrix *matrix, QList<quint32> *inputData, int inputWidth, int inputHeight, QList<quint32> *outputData, int *outputWidth, int *outputHeight)
{
    *outputHeight = inputHeight;
    outputData->clear();

    int resultWidth = 0;

    // each row
    for (int y = 0; y < inputHeight; y++)
    {
        // start of row in inputData
        int start = y * inputWidth;
        // get row data packed
        QList<quint32> rowData;
        ConverterHelper::packDataRow(matrix, inputData, start, inputWidth, &rowData);
        outputData->append(rowData);
        // get row blocks count
        resultWidth = qMax(resultWidth, rowData.length());
    }
    *outputWidth = resultWidth;
}
//-----------------------------------------------------------------------------
void ConverterHelper::prepareImage(ConversionMatrix *matrix, QImage *source, QImage *result)
{
    if (source != NULL)
    {
        QImage im = *source;

        switch (matrix->options()->rotate())
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
        if (matrix->options()->flipHorizontal())
            im = BitmapHelper::flipHorizontal(&im);
        if (matrix->options()->flipVertical())
            im = BitmapHelper::flipVertical(&im);
        if (matrix->options()->inverse())
            im.invertPixels();

        *result = im;
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
void ConverterHelper::packDataRow(ConversionMatrix *matrix, QList<quint32> *inputData, int start, int count, QList<quint32> *outputData)
{
    if (matrix != NULL && inputData != NULL && outputData != NULL)
    {
        outputData->clear();

        BitStream stream(matrix, inputData, start, count);
        while (!stream.eof())
        {
            quint32 value = stream.next();

            if (matrix->options()->bytesOrder() == BytesOrderBigEndian)
                value = ConverterHelper::toBigEndian(matrix, value);

            outputData->append(value);
        }
    }
}
//-----------------------------------------------------------------------------
quint32 ConverterHelper::toBigEndian(ConversionMatrix *matrix, quint32 value)
{
    quint8 src1, src2, src3, src4;
    src1 = value & 0xff;
    src2 = (value >> 8) & 0xff;
    src3 = (value >> 16) & 0xff;
    src4 = (value >> 24) & 0xff;

    quint32 result = 0;

    switch (matrix->options()->blockSize())
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
