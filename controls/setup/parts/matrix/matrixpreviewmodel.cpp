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

#include "matrixpreviewmodel.h"

#include <QColor>
#include "preset.h"
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "imageoptions.h"

MatrixPreviewModel::MatrixPreviewModel(Preset *preset, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mPreset = preset;

    this->connect(this->mPreset, SIGNAL(changed()), SLOT(callReset()));
}

int MatrixPreviewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int result = this->mPreset->matrix()->operationsCount();
    result += 7; // source, and, or, fill, used, result, result packed
    return result;
}

int MatrixPreviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 32;
}

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
                result = QString("AND");
                break;
            case MaskOr:
                result = QString("OR");
                break;
            case Result:
                result = tr("Result");
                break;
            case ResultPacked:
                result = tr("Stream");
                break;
            case MaskFill:
                result = tr("Fill");
                break;
            default:
                result = QString("");
            }
        }
    }
    return result;
}

QVariant MatrixPreviewModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

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
            int shift;
            quint32 mask;
            bool left;

            this->mPreset->matrix()->operation(row, &mask, &shift, &left);

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
            bool active = (this->mPreset->matrix()->maskUsed() & (0x00000001 << bitIndex)) != 0;

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
            bool active = (this->mPreset->matrix()->maskAnd() & (0x00000001 << bitIndex)) != 0;

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
            bool active = (this->mPreset->matrix()->maskOr() & (0x00000001 << bitIndex)) != 0;

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
        case MaskFill:
        {
            bool active = (this->mPreset->matrix()->maskFill() & (0x00000001 << bitIndex)) != 0;
            int bits = 8 * (this->mPreset->image()->blockSize() + 1);

            if (role == Qt::DisplayRole)
            {
                if (bitIndex < bits)
                    result = QString("%1").arg(active ? 1 : 0);
            }
            else if (role == Qt::BackgroundColorRole)
            {
                if (!active || (bitIndex >= bits))
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
        case ResultPacked:
        {
            if (role == Qt::DisplayRole)
            {
                QVariant name, color;
                this->resultPackedToSourceBit(bitIndex, &name, &color);
                result = name;
            }
            else if (role == Qt::BackgroundColorRole)
            {
                QVariant name, color;
                this->resultPackedToSourceBit(bitIndex, &name, &color);
                result = color;
            }
            break;
        }
        }
    }

    return result;
}

bool MatrixPreviewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        int bitIndex = 31 - index.column();
        RowType type = this->rowType(index.row());

        if (role == Qt::EditRole &&
                (type == MaskUsed || type == MaskAnd || type == MaskOr || type == MaskFill))
        {
            bool ok;
            bool bit = value.toInt(&ok);
            if (ok)
            {
                quint32 mask = 0;
                switch (type)
                {
                case MaskUsed:
                    mask = this->mPreset->matrix()->maskUsed();
                    break;
                case MaskAnd:
                    mask = this->mPreset->matrix()->maskAnd();
                    break;
                case MaskOr:
                    mask = this->mPreset->matrix()->maskOr();
                    break;
                case MaskFill:
                    mask = this->mPreset->matrix()->maskFill();
                    break;
                default:
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
                    this->mPreset->matrix()->setMaskUsed(mask);
                    break;
                case MaskAnd:
                    this->mPreset->matrix()->setMaskAnd(mask);
                    break;
                case MaskOr:
                    this->mPreset->matrix()->setMaskOr(mask);
                    break;
                case MaskFill:
                    this->mPreset->matrix()->setMaskFill(mask);
                    break;
                default:
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

QModelIndex MatrixPreviewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}

QModelIndex MatrixPreviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

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
        case MaskFill:
            flags |= Qt::ItemIsEditable;
            break;
        default:
            break;
        }
    }
    return flags;
}

MatrixPreviewModel::RowType MatrixPreviewModel::rowType(int row) const
{
    MatrixPreviewModel::RowType result = Operation;
    int rows = this->rowCount(QModelIndex());

    if (row == 0)
        result = Source;
    else if (row == rows - 1)
        result = ResultPacked;
    else if (row == rows - 2)
        result = Result;
    else if (row == rows - 3)
        result = MaskFill;
    else if (row == rows - 4)
        result = MaskOr;
    else if (row == rows - 5)
        result = MaskAnd;
    else if (row == rows - 6)
        result = MaskUsed;

    return result;
}

void MatrixPreviewModel::getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const
{
    *convType = this->mPreset->prepare()->convType();
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

    if (bitIndex >= 24)
    {
        *colorType = Alpha;
        *partIndex = bitIndex & 7;
    }
}

void MatrixPreviewModel::resultToSourceBit(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    // check bit using
    bool active = (this->mPreset->matrix()->maskUsed() & (0x00000001 << bitIndex)) != 0;
    if (active)
    {
        // check bit OR
        bool bitOr = (this->mPreset->matrix()->maskOr() & (0x00000001 << bitIndex)) != 0;
        if (!bitOr)
        {
            // check bit AND
            bool bitAnd = (this->mPreset->matrix()->maskAnd() & (0x00000001 << bitIndex)) != 0;
            if (bitAnd)
            {
                // by default
                *name = 0;

                // find source bit before shifting
                for (int i = this->mPreset->matrix()->operationsCount() - 1; i >= 0; i--)
                {
                    quint32 mask;
                    int shift;
                    bool left;

                    this->mPreset->matrix()->operation(i, &mask, &shift, &left);

                    // get source bit index
                    int sourceBitIndex = bitIndex;
                    if (left)
                        sourceBitIndex -= (int)shift;
                    else
                        sourceBitIndex += (int)shift;

                    if ((mask & (0x01 << sourceBitIndex)) != 0 && (sourceBitIndex >= 0) && (sourceBitIndex <= 31))
                    {
                        *name = this->data(this->createIndex(0, 31 - sourceBitIndex), Qt::DisplayRole);
                        this->sourceBitProperties(sourceBitIndex, name, color);

                        break;
                    }
                }

                if (this->mPreset->matrix()->operationsCount() == 0)
                {
                    *name = this->data(this->createIndex(0, 31 - bitIndex), Qt::DisplayRole);
                    this->sourceBitProperties(bitIndex, name, color);
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

void MatrixPreviewModel::resultPackedToSourceBit(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    bool active = (this->mPreset->matrix()->maskFill() & (0x00000001 << bitIndex)) != 0;

    if (!active)
    {
        *name = QVariant("0");
        *color = QVariant(QColor(50, 50, 50, 200));
    }
    else
    {
        // count of active bits in "Used" mask
        int usedBitsCount = 0;
        quint32 mask = this->mPreset->matrix()->maskUsed();

        while (mask != 0)
        {
            if ((mask & 0x00000001) != 0)
                usedBitsCount ++;
            mask = mask >> 1;
        }

        // index of current bit in active bits of "Fill" mask, from MSB
        int fillBitIndex = -1;

        mask = this->mPreset->matrix()->maskFill();

        for (int i = 31; i >= bitIndex; i--)
        {
            if ((mask & (0x00000001 << i)) != 0)
                fillBitIndex++;
        }

        fillBitIndex = fillBitIndex % usedBitsCount;

        // find index of bit in "Used" mask
        int usedBitIndex = 31;
        mask = this->mPreset->matrix()->maskUsed();
        for (int i = 31; (i >= 0) && (fillBitIndex >= 0); i--)
        {
            if ((mask & (0x00000001 << i)) != 0)
            {
                usedBitIndex = i;
                fillBitIndex--;
            }
        }

        this->resultToSourceBit(usedBitIndex, name, color);
    }
}

void MatrixPreviewModel::sourceBitProperties(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    if (bitIndex >= 0 && bitIndex <= 31)
    {
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
                switch (colorType)
                {
                case BlackOrWhite:
                {
                    *name = QVariant(QString("BW%1").arg(partIndex));
                    *color = QVariant(QColor(128, 128, 128));
                    break;
                }
                case Alpha:
                {
                    *name = QVariant(QString("A%1").arg(partIndex));
                    int a = (80 / 8 * partIndex) + 50;
                    *color = QVariant(QColor(128, 128, 128, a));
                    break;
                }
                default:
                {
                    break;
                }
                }
                break;
            }
            case ConversionTypeGrayscale:
            {
                switch (colorType)
                {
                case Gray:
                {
                    *name = QVariant(QString("Gr%1").arg(partIndex));
                    int a = (80 / 8 * partIndex) + 50;
                    *color = QVariant(QColor(10, 10, 10, a));
                    break;
                }
                case Alpha:
                {
                    *name = QVariant(QString("A%1").arg(partIndex));
                    int a = (80 / 8 * partIndex) + 50;
                    *color = QVariant(QColor(128, 128, 128, a));
                    break;
                }
                default:
                {
                    break;
                }
                }
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
                case Alpha:
                {
                    *name = QVariant(QString("A%1").arg(partIndex));
                    int a = (80 / 8 * partIndex) + 50;
                    *color = QVariant(QColor(128, 128, 128, a));
                    break;
                }
                default:
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
}

void MatrixPreviewModel::callReset()
{
    this->beginResetModel();
    this->endResetModel();
}

