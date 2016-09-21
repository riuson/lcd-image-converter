/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "imagesmodel.h"
#include <QPixmap>
#include "datacontainer.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
ImagesModel::ImagesModel(DataContainer *container, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mContainer = container;

    this->setCrop(0, 0, 0, 0);

    this->connect(this->mContainer, SIGNAL(dataChanged(bool)), SLOT(imagesChanged()));
}
//-----------------------------------------------------------------------------
int ImagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return this->mContainer->count();
}
//-----------------------------------------------------------------------------
int ImagesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 2;
}
//-----------------------------------------------------------------------------
QVariant ImagesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            result = this->containerValue(section, KeyCodeRole);
        }
        else
        {
            switch (section)
            {
            case 0:
            {
                if (this->rowCount(QModelIndex()) > 1)
                {
                    result = tr("Character");
                }
                break;
            }
            case 1:
            {
                result = tr("Preview", "character preview");
                break;
            }
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant ImagesModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();

    if (!index.isValid())
        return result;

    int columnIndex = index.column();
    int valueIndex = index.row();

    switch (role)
    {
    case Qt::DisplayRole:
    {
        if (columnIndex == 0)
        {
            result = this->containerValue(valueIndex, KeyRole);
        }
        break;
    }
    case Qt::DecorationRole:
    {
        if (columnIndex == 1)
        {
            result = this->containerValue(valueIndex, ImageRole);
        }
        break;
    }
    case Qt::SizeHintRole:
    {
        if (columnIndex == 1)
        {
            QSize size = this->containerValueSize(valueIndex, ImageRole);
            result = size;
        }
        break;
    }
    case Qt::TextAlignmentRole:
    {
        if (columnIndex == 0)
        {
            result = Qt::AlignCenter;
        }
        break;
    }
    case KeyRole:
    case KeyCodeRole:
    case ImageRole:
    {
        result = this->containerValue(valueIndex, (ImagesModelRoles)role);
        break;
    }
    default:
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
QModelIndex ImagesModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex ImagesModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
void ImagesModel::callReset()
{
    this->beginResetModel();
    this->endResetModel();
}
//-----------------------------------------------------------------------------
int ImagesModel::scale() const
{
    return 1;
}
//-----------------------------------------------------------------------------
void ImagesModel::setScale(int value)
{
    Q_UNUSED(value)
}
//-----------------------------------------------------------------------------
void ImagesModel::setCrop(int left, int top, int right, int bottom)
{
    Q_UNUSED(left)
    Q_UNUSED(top)
    Q_UNUSED(right)
    Q_UNUSED(bottom)
}
//-----------------------------------------------------------------------------
QVariant ImagesModel::containerValue(int imageIndex, ImagesModelRoles role) const
{
    QVariant result;

    if (imageIndex >= 0 && imageIndex < this->mContainer->count())
    {
        QString key = this->mContainer->keys().at(imageIndex);

        switch (role)
        {
        case KeyRole:
        {
            result = key;
            break;
        }
        case KeyCodeRole:
        {
            quint16 code = key.at(0).unicode();
            result = QString("U+%1").arg(code, 4, 16, QChar('0'));
            break;
        }
        case ImageRole:
        {
            const QImage *source = this->mContainer->image(key);
            result = QImage(*source);
            break;
        }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
QSize ImagesModel::containerValueSize(int imageIndex, ImagesModelRoles role) const
{
    QSize result = QSize();

    if (imageIndex >= 0 && imageIndex < this->mContainer->count())
    {
        QString key = this->mContainer->keys().at(imageIndex);

        const QImage *source = this->mContainer->image(key);

        switch (role)
        {
        case ImageRole:
        {
            result = source->size();
            break;
        }
        default:
            break;
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
void ImagesModel::imagesChanged()
{
    emit this->dataChanged(
                this->index(0, 0),
                this->index(this->rowCount(QModelIndex()) - 1, this->columnCount(QModelIndex()) - 1));
}
//-----------------------------------------------------------------------------
