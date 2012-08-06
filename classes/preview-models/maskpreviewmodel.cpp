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

#include "maskpreviewmodel.h"
//-----------------------------------------------------------------------------
#include <QColor>
#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
MaskPreviewModel::MaskPreviewModel(QList<quint32> *matrix, int maskIndex, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mMatrix = matrix;
    this->mMaskIndex = maskIndex;
}
//-----------------------------------------------------------------------------
int MaskPreviewModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
//-----------------------------------------------------------------------------
int MaskPreviewModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 32;
}
//-----------------------------------------------------------------------------
QVariant MaskPreviewModel::headerData(int section, Qt::Orientation orientation, int role) const
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
QVariant MaskPreviewModel::data(const QModelIndex &index, int role) const
{
    QVariant result;

    if (index.isValid())
    {
        int bitIndex = 31 - index.column();
        bool active = (this->mMatrix->at(this->mMaskIndex) & (0x00000001 << bitIndex)) != 0;

        if (role == Qt::DisplayRole)
        {
            result = QString("%1").arg(active ? 1 : 0);
        }
        else if (role == Qt::BackgroundColorRole)
        {
            if (active)
                result = QVariant(QColor(255, 255, 255, 255));
            else
                result = QVariant(QColor(50, 50, 50, 200));
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
bool MaskPreviewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        int bitIndex = 31 - index.column();

        if (role == Qt::EditRole)
        {
            bool ok;
            bool bit = value.toInt(&ok);
            if (ok)
            {
                quint32 mask = this->mMatrix->at(this->mMaskIndex);
                quint32 maskEdit = 0x00000001 << bitIndex;
                if (bit)
                    mask |= maskEdit;
                else
                    mask &= ~maskEdit;
                this->mMatrix->replace(this->mMaskIndex, mask);
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
QModelIndex MaskPreviewModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex MaskPreviewModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
Qt::ItemFlags MaskPreviewModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags(index);
    flags |= Qt::ItemIsEditable;
    return flags;
}
//-----------------------------------------------------------------------------
