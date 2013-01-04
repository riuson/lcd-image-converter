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

#include "reorderingpreviewmodel.h"
//-----------------------------------------------------------------------------
#include <QColor>
#include "preset.h"
#include "prepareoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
//-----------------------------------------------------------------------------
ReorderingPreviewModel::ReorderingPreviewModel(Preset *preset, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mPreset = preset;

    this->connect(this->mPreset, SIGNAL(changed()), SLOT(callReset()));
}
//-----------------------------------------------------------------------------
int ReorderingPreviewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    int result = this->mPreset->reordering()->operationsCount();
    result += 2; // source, result
    return result;
}
//-----------------------------------------------------------------------------
int ReorderingPreviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 32;
}
//-----------------------------------------------------------------------------
QVariant ReorderingPreviewModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case Result:
                result = tr("Result");
                break;
            default:
                result = QString("");
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant ReorderingPreviewModel::data(const QModelIndex &index, int role) const
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

            this->mPreset->reordering()->operation(row, &mask, &shift, &left);

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
bool ReorderingPreviewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}
//-----------------------------------------------------------------------------
QModelIndex ReorderingPreviewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex ReorderingPreviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
Qt::ItemFlags ReorderingPreviewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    if (index.isValid())
    {
        switch (this->rowType(index.row()))
        {
        default:
            break;
        }
    }
    return flags;
}
//-----------------------------------------------------------------------------
ReorderingPreviewModel::RowType ReorderingPreviewModel::rowType(int row) const
{
    ReorderingPreviewModel::RowType result = Operation;
    int rows = this->rowCount(QModelIndex());

    if (row == 0)
        result = Source;
    else if (row == rows - 1)
        result = Result;

    return result;
}
//-----------------------------------------------------------------------------
void ReorderingPreviewModel::getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const
{
    int bits = (((int)this->mPreset->image()->blockSize()) * 8) + 8;
    *convType = this->mPreset->prepare()->convType();
    *colorType = Empty;
    *partIndex = 0;

    if (bitIndex < bits)
    {
        *colorType = Gray;
        *partIndex = bitIndex;
    }
}
//-----------------------------------------------------------------------------
void ReorderingPreviewModel::resultToSourceBit(int bitIndex, QVariant *name, QVariant *color) const
{
    *name = QVariant();
    *color = QVariant();

    // by default
    *name = 0;

    // find source bit before shifting
    for (int i = this->mPreset->reordering()->operationsCount() - 1; i >= 0; i--)
    {
        quint32 mask;
        int shift;
        bool left;

        this->mPreset->reordering()->operation(i, &mask, &shift, &left);

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

    if (this->mPreset->reordering()->operationsCount() == 0)
    {
        *name = this->data(this->createIndex(0, 31 - bitIndex), Qt::DisplayRole);
        this->sourceBitProperties(bitIndex, name, color);
    }
}
//-----------------------------------------------------------------------------
void ReorderingPreviewModel::sourceBitProperties(int bitIndex, QVariant *name, QVariant *color) const
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
            *name = QVariant(QString("%1").arg(partIndex));
            int a = (80 / 8 * partIndex) + 50;
            *color = QVariant(QColor(10, 10, 10, a));
        }
    }
}
//-----------------------------------------------------------------------------
void ReorderingPreviewModel::callReset()
{
    this->reset();
}
//-----------------------------------------------------------------------------
