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

#include "matrixpreviewmodel.h"
//-----------------------------------------------------------------------------
#include <QColor>
//-----------------------------------------------------------------------------
MatrixPreviewModel::MatrixPreviewModel(QList<quint32> *matrix, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mMatrix = matrix;
}
//-----------------------------------------------------------------------------
int MatrixPreviewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int result = this->mMatrix->length() - ConversionMatrixOptions::OperationsStartIndex;
    result = result >> 1;
    result += 5;// rows for: source, used, and, or, result
    return result;
}
//-----------------------------------------------------------------------------
int MatrixPreviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 32;
}
//-----------------------------------------------------------------------------
QVariant MatrixPreviewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result = QAbstractItemModel::headerData(section, orientation, role);

    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            result = QString("%1").arg(31 - section, 2);
        }
        if (orientation == Qt::Vertical)
        {
            switch (this->rowType(section))
            {
            case Source:
                result = tr("Source");
                break;
            case Operation:
                result = tr("Operation %1").arg(section);
                break;
            case MaskUsed:
                result = tr("Used");
                break;
            case MaskAnd:
                result = tr("AND");
                break;
            case MaskOr:
                result = tr("OR");
                break;
            case Result:
                result = tr("Result");
                break;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant MatrixPreviewModel::data(const QModelIndex &index, int role) const
{
    QVariant result;
    ConversionMatrixOptions options(this->mMatrix);

    if (index.isValid())
    {
        int bitIndex = 31 - index.column();
        int row = index.row();

        switch (this->rowType(index.row()))
        {
        case Operation:
        {
            // get source bit index
            row--;
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
            if ((mask & (0x01 << bitIndex)) != 0)
            {
                if (role == Qt::DisplayRole)
                {
                    QVariant name, color;
                    this->sourceBitProperties(bitIndex, &name, &color);
                    result = name;
                }
                else if (role == Qt::BackgroundColorRole)
                {
                    QVariant name, color;
                    this->sourceBitProperties(bitIndex, &name, &color);
                    result = color;
                }
            }
            break;
        }
        case Source:
        {
            ConversionType convType;
            ColorType colorType;
            int partIndex;
            this->getBitType(bitIndex, &convType, &colorType, &partIndex);

            if (role == Qt::DisplayRole)
            {
                QVariant name, color;
                this->sourceBitProperties(bitIndex, &name, &color);
                result = name;
                            }
            else if (role == Qt::BackgroundColorRole)
            {
                QVariant name, color;
                this->sourceBitProperties(bitIndex, &name, &color);
                result = color;
            }
            break;
        }
        case MaskUsed:
        {
            bool active = (options.maskUsed() & (0x00000001 << bitIndex)) != 0;

            if (role == Qt::DisplayRole)
            {
                result = QString("%1").arg(active ? 1 : 0);
            }
            else if (role == Qt::BackgroundColorRole)
            {
                if (!active)
                    result = QVariant(QColor(50, 50, 50, 200));
            }
            break;
        }
        case MaskAnd:
        {
            bool active = (options.maskAnd() & (0x00000001 << bitIndex)) != 0;

            if (role == Qt::DisplayRole)
            {
                result = QString("%1").arg(active ? 1 : 0);
            }
            else if (role == Qt::BackgroundColorRole)
            {
                if (!active)
                    result = QVariant(QColor(50, 50, 50, 200));
            }
            break;
        }
        case MaskOr:
        {
            bool active = (options.maskOr() & (0x00000001 << bitIndex)) != 0;

            if (role == Qt::DisplayRole)
            {
                result = QString("%1").arg(active ? 1 : 0);
            }
            else if (role == Qt::BackgroundColorRole)
            {
                if (!active)
                    result = QVariant(QColor(50, 50, 50, 200));
            }
            break;
        }
        case Result:
        {
            if (role == Qt::DisplayRole)
            {
                QVariant name, color;
                this->resultToSourceBit(bitIndex, &name, &color);
                result = name;
            }
            else if (role == Qt::BackgroundColorRole)
            {
                QVariant name, color;
                this->resultToSourceBit(bitIndex, &name, &color);
                result = color;
            }
            break;
        }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
bool MatrixPreviewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        int bitIndex = 31 - index.column();
        RowType type = this->rowType(index.row());

        if (role == Qt::EditRole &&
                (type == MaskUsed || type == MaskAnd || type == MaskOr))
        {
            bool ok;
            bool bit = value.toInt(&ok);
            if (ok)
            {
                ConversionMatrixOptions options(this->mMatrix);

                quint32 mask = 0;
                switch (type)
                {
                case MaskUsed:
                    mask = options.maskUsed();
                    break;
                case MaskAnd:
                    mask = options.maskAnd();
                    break;
                case MaskOr:
                    mask = options.maskOr();
                    break;
                case Source:
                case Operation:
                case Result:
                    break;
                }

                quint32 maskEdit = 0x00000001 << bitIndex;
                if (bit)
                    mask |= maskEdit;
                else
                    mask &= ~maskEdit;

                switch (type)
                {
                case MaskUsed:
                    options.setMaskUsed(mask);
                    break;
                case MaskAnd:
                    options.setMaskAnd(mask);
                    break;
                case MaskOr:
                    options.setMaskOr(mask);
                    break;
                case Source:
                case Operation:
                case Result:
                    break;
                }

                this->dataChanged(index, index);

                return true;
            }
            else
                return false;
        }
    }
    return false;
}
//-----------------------------------------------------------------------------
QModelIndex MatrixPreviewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex MatrixPreviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
Qt::ItemFlags MatrixPreviewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        switch (this->rowType(index.row()))
        {
        case MaskUsed:
        case MaskAnd:
        case MaskOr:
            flags |= Qt::ItemIsEditable;
            break;
        default:
            break;
        }
    }
    return flags;
}
//-----------------------------------------------------------------------------
void MatrixPreviewModel::callReset()
{
    this->reset();
}
//-----------------------------------------------------------------------------
void MatrixPreviewModel::getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const
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
MatrixPreviewModel::RowType MatrixPreviewModel::rowType(int row) const
{
    MatrixPreviewModel::RowType result = Operation;
    int rows = this->rowCount(QModelIndex());

    if (row == 0)
        result = Source;
    else if (row == rows - 1)
        result = Result;
    else if (row == rows - 2)
        result = MaskOr;
    else if (row == rows - 3)
        result = MaskAnd;
    else if (row == rows - 4)
        result = MaskUsed;

    return result;
}
//-----------------------------------------------------------------------------
void MatrixPreviewModel::resultToSourceBit(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    ConversionMatrixOptions options(this->mMatrix);

    // check bit using
    bool active = (options.maskUsed() & (0x00000001 << bitIndex)) != 0;
    if (active)
    {
        // check bit OR
        bool bitOr = (options.maskOr() & (0x00000001 << bitIndex)) != 0;
        if (!bitOr)
        {
            // check bit AND
            bool bitAnd = (options.maskAnd() & (0x00000001 << bitIndex)) != 0;
            if (bitAnd)
            {
                // by default
                *name = 0;

                // find source bit before shifting
                int ops = this->mMatrix->length() - ConversionMatrixOptions::OperationsStartIndex;
                ops = ops >> 1;
                for (int i = ops - 1; i >= 0; i--)
                {
                    quint32 mask = this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (i << 1));

                    quint32 shift = this->mMatrix->at(ConversionMatrixOptions::OperationsStartIndex + (i << 1) + 1);
                    bool left = (shift & 0x80000000) != 0;
                    shift &= 0x0000001f;

                    // get source bit index
                    int sourceBitIndex = bitIndex;
                    if (left)
                        sourceBitIndex -= (int)shift;
                    else
                        sourceBitIndex += (int)shift;

                    if ((mask & (0x01 << sourceBitIndex)) != 0)
                    {
                        *name = this->data(this->createIndex(0, 31 - sourceBitIndex), Qt::DisplayRole);

                        this->sourceBitProperties(sourceBitIndex, name, color);

                        break;
                    }
                }
            }
            else
                *name = "0";
        }
        else
            *name = "1";
    }
    else
    {
        *color = QVariant(QColor(50, 50, 50, 200));
    }
}
//-----------------------------------------------------------------------------
void MatrixPreviewModel::sourceBitProperties(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    ConversionType convType;
    ColorType colorType;
    int partIndex;
    this->getBitType(bitIndex, &convType, &colorType, &partIndex);

    if (colorType != Empty)
    {
        switch (convType)
        {
        case ConversionTypeMonochrome:
        {
            *name = QVariant(QString("BW%1").arg(partIndex));
            *color = QVariant(QColor(128, 128, 128));
            break;
        }
        case ConversionTypeGrayscale:
        {
            *name = QVariant(QString("Gr%1").arg(partIndex));
            int a = (80 / 8 * partIndex) + 50;
            *color = QVariant(QColor(10, 10, 10, a));
            break;
        }
        case ConversionTypeColor:
        {
            switch (colorType)
            {
            case Red:
            {
                *name = QVariant(QString("R%1").arg(partIndex));
                int a = (80 / 8 * partIndex) + 50;
                *color = QVariant(QColor(255, 0, 0, a));
                break;
            }
            case Green:
            {
                *name = QVariant(QString("G%1").arg(partIndex));
                int a = (80 / 8 * partIndex) + 50;
                *color = QVariant(QColor(0, 255, 0, a));
                break;
            }
            case Blue:
            {
                *name = QVariant(QString("B%1").arg(partIndex));
                int a = (80 / 8 * partIndex) + 50;
                *color = QVariant(QColor(0, 0, 255, a));
                break;
            }
            case Empty:
            case BlackOrWhite:
            case Gray:
                break;
            }

            break;
        }
        }
    }
    else
    {
        //*color = QVariant(QColor(50, 50, 50, 200));
        *name = QVariant(QString("0"));
    }
}
//-----------------------------------------------------------------------------
