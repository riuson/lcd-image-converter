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
#include "bitmapeditoroptions.h"
//-----------------------------------------------------------------------------
ImagesModel::ImagesModel(DataContainer *container, Qt::Orientation orientation, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mContainer = container;
    this->mScale = 2;
    this->mOrientation = orientation;

    this->setCrop(0, 0, 0, 0);

    this->connect(this->mContainer, SIGNAL(imagesChanged()), SLOT(imagesChanged()));
}
//-----------------------------------------------------------------------------
int ImagesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (this->mOrientation == Qt::Vertical)
    {
        return this->mContainer->count();
    }
    else
    {
        return 2;
    }
}
//-----------------------------------------------------------------------------
int ImagesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (this->mOrientation == Qt::Vertical)
    {
        return 2;
    }
    else
    {
        return this->mContainer->count();
    }
}
//-----------------------------------------------------------------------------
QVariant ImagesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        if (this->mOrientation == Qt::Vertical)
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
        else
        {
            if (orientation == Qt::Horizontal)
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

    // swap for horizontal
    if (this->mOrientation == Qt::Horizontal)
    {
        columnIndex = index.row();
        valueIndex = index.column();
    }

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
            result = this->containerValue(valueIndex, PixmapScaledCroppedRole);
        }
        break;
    }
    case Qt::SizeHintRole:
    {
        if (columnIndex == 1)
        {
            QVariant var = this->containerValue(valueIndex, PixmapScaledCroppedRole);
            QPixmap pixmap = var.value<QPixmap>();

            QSize size = pixmap.size();
            size.rheight() += 10;
            size.rwidth() += 10;

            result = size;
        }
        break;
    }
    case KeyRole:
    case KeyCodeRole:
    case ImageRole:
    case ImageScaledRole:
    case PixmapRole:
    case PixmapScaledRole:
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
    this->reset();
}
//-----------------------------------------------------------------------------
int ImagesModel::scale() const
{
    return this->mScale;
}
//-----------------------------------------------------------------------------
void ImagesModel::setScale(int value)
{
    if (value >= 1)
    {
        this->mScale = value;
        emit this->scaleChanged();
    }
}
//-----------------------------------------------------------------------------
void ImagesModel::setCrop(int left, int top, int right, int bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;

    this->imagesChanged();
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
        case PixmapScaledCroppedRole:
        {
            const QImage *source = this->mContainer->image(key);

            QImage cropped = BitmapHelper::crop(source, this->mLeft, this->mTop, this->mRight, this->mBottom, BitmapEditorOptions::color2());
            QImage scaled = BitmapHelper::scale(&cropped, this->mScale);
            QImage grids = BitmapHelper::drawGrid(&scaled, this->mScale);

            result = QPixmap::fromImage(grids);
            break;
        }
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
