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

#include "imagesmodelvertical.h"
#include <QPixmap>
#include "datacontainer.h"
#include "bitmaphelper.h"
//-----------------------------------------------------------------------------
ImagesModelVertical::ImagesModelVertical(DataContainer *container, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mContainer = container;
    this->mScale = 2;
}
//-----------------------------------------------------------------------------
int ImagesModelVertical::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->mContainer->count();
}
//-----------------------------------------------------------------------------
int ImagesModelVertical::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
//-----------------------------------------------------------------------------
QVariant ImagesModelVertical::headerData(int section, Qt::Orientation orientation, int role) const
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
                result = tr("Character");
                break;
            case 1:
                result = tr("Preview", "character prewview");
                break;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant ImagesModelVertical::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();

    if (!index.isValid())
        return result;

    if (role == Qt::DisplayRole)
    {
        if (index.column() == 0)
        {
            result = this->containerValue(index.row(), KeyRole);
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.column() == 1)
        {
            result = this->containerValue(index.row(), PixmapScaledRole);
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        if (index.column() == 1)
        {
            QVariant var = this->containerValue(index.row(), PixmapScaledRole);
            QPixmap pixmap = var.value<QPixmap>();
            result = pixmap.size();
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QModelIndex ImagesModelVertical::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex ImagesModelVertical::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
void ImagesModelVertical::callReset()
{
    this->reset();
}
//-----------------------------------------------------------------------------
int ImagesModelVertical::scale() const
{
    return this->mScale;
}
//-----------------------------------------------------------------------------
void ImagesModelVertical::setScale(int value)
{
    this->mScale = value;
    emit this->dataChanged(
                this->index(0, 0),
                this->index(this->mContainer->count() - 1, 1));
}
//-----------------------------------------------------------------------------
QVariant ImagesModelVertical::containerValue(int imageIndex, ImagesModelRoles role) const
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
        case ImageScaledRole:
        {
            const QImage *source = this->mContainer->image(key);

            QImage scaled = BitmapHelper::scale(source, this->mScale);
            QImage grids = BitmapHelper::drawGrid(&scaled, this->mScale);

            result = grids;
            break;
        }
        case PixmapRole:
        {
            const QImage *source = this->mContainer->image(key);
            result = QPixmap::fromImage(*source);
            break;
        }
        case PixmapScaledRole:
        {
            const QImage *source = this->mContainer->image(key);

            QImage scaled = BitmapHelper::scale(source, this->mScale);
            QImage grids = BitmapHelper::drawGrid(&scaled, this->mScale);

            result = QPixmap::fromImage(grids);
            break;
        }
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
