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

#include "operationspreviewmodel.h"
//-----------------------------------------------------------------------------
#include <QColor>
#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
OperationsPreviewModel::OperationsPreviewModel(QList<quint32> *matrix, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mMatrix = matrix;
}
//-----------------------------------------------------------------------------
int OperationsPreviewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int result = this->mMatrix->length() - ConversionMatrixOptions::OperationsStartIndex;
    return result / 2;
}
//-----------------------------------------------------------------------------
int OperationsPreviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 32;
}
//-----------------------------------------------------------------------------
QVariant OperationsPreviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result = QAbstractItemModel::headerData(section, orientation, role);

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            result = QString("%1").arg(31 - section, 2);
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant OperationsPreviewModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        int bitIndex = 31 - index.column();
        int row = index.row();

        // get source bit index
        quint32 shift = this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (row << 1) + 1);
        bool left = (shift & 0x80000000) != 0;
        shift &= 0x0000001f;
        if (left)
            bitIndex -= (int)shift;
        else
            bitIndex += (int)shift;

        // get source bit info
        ConversionType convType;
        ColorType colorType;
        int partIndex;
        this->getBitType(bitIndex, &convType, &colorType, &partIndex);

        // check for bit using
        quint32 mask = this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (row << 1));
        if ((mask & (0x01 << bitIndex)) == 0)
            colorType = Empty;

        if (role == Qt::DisplayRole)
        {
            if (colorType != Empty)
            {
                switch (convType)
                {
                case ConversionTypeMonochrome:
                    result = QString("BW%1").arg(partIndex);
                    break;
                case ConversionTypeGrayscale:
                    result = QString("Gr%1").arg(partIndex);
                    break;
                case ConversionTypeColor:
                {
                    switch (colorType)
                    {
                    case Red:
                        result = QString("R%1").arg(partIndex);
                        break;
                    case Green:
                        result = QString("G%1").arg(partIndex);
                        break;
                    case Blue:
                        result = QString("B%1").arg(partIndex);
                        break;
                    }

                    break;
                }
                }
            }
        }
        else if (role == Qt::BackgroundColorRole)
        {
            if (colorType != Empty)
            {
                switch (convType)
                {
                case ConversionTypeMonochrome:
                {
                    result = QVariant(QColor(128, 128, 128));
                    break;
                }
                case ConversionTypeGrayscale:
                {
                    int a = (80 / 8 * partIndex) + 50;
                    result = QVariant(QColor(10, 10, 10, a));
                    break;
                }
                case ConversionTypeColor:
                {
                    switch (colorType)
                    {
                    case Red:
                    {
                        int a = (80 / 8 * partIndex) + 50;
                        result = QVariant(QColor(255, 0, 0, a));
                        break;
                    }
                    case Green:
                    {
                        int a = (80 / 8 * partIndex) + 50;
                        result = QVariant(QColor(0, 255, 0, a));
                        break;
                    }
                    case Blue:
                    {
                        int a = (80 / 8 * partIndex) + 50;
                        result = QVariant(QColor(0, 0, 255, a));
                        break;
                    }
                    }

                    break;
                }
                }
            }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
QModelIndex OperationsPreviewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex OperationsPreviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
void OperationsPreviewModel::getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const
{
    ConversionMatrixOptions options(this->mMatrix);

    *convType = options.convType();
    *colorType = Empty;
    *partIndex = 0;

    switch (*convType)
    {
    case ConversionTypeMonochrome:
    {
        if (bitIndex < 24)
        {
            *colorType = BlackOrWhite;
            *partIndex = 0;
        }

        break;
    }
    case ConversionTypeGrayscale:
    {
        if (bitIndex < 24)
        {
            *colorType = Gray;
            *partIndex = bitIndex & 7;
        }

        break;
    }
    case ConversionTypeColor:
    {
        if (bitIndex < 8)
        {
            *colorType = Blue;
            *partIndex = bitIndex & 7;
        }
        else if (bitIndex < 16)
        {
            *colorType = Green;
            *partIndex = bitIndex & 7;
        }
        else if (bitIndex < 24)
        {
            *colorType = Red;
            *partIndex = bitIndex & 7;
        }

        break;
    }
    }
}
//-----------------------------------------------------------------------------
