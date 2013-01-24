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

#include "resizemodel.h"
//-----------------------------------------------------------------------------
#include <QPixmap>
#include "datacontainer.h"
#include "bitmaphelper.h"
#include "bitmapeditoroptions.h"
//-----------------------------------------------------------------------------
ResizeModel::ResizeModel(DataContainer *container, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mContainer = container;
    this->setCrop(0, 0, 0, 0);
    this->setScale(1);
}
//-----------------------------------------------------------------------------
int ResizeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
//-----------------------------------------------------------------------------
int ResizeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->mContainer->count();
}
//-----------------------------------------------------------------------------
QVariant ResizeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section < this->mContainer->count())
            {
                quint16 code = this->mContainer->keys().at(section).at(0).unicode();
                result = QString("U+%1").arg(code, 4, 16, QChar('0'));
            }
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
QVariant ResizeModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();
    if (!index.isValid())
        return result;

    if (role == Qt::DisplayRole)
    {
        if (index.row() == 0)
        {
            QString key = this->mContainer->keys().at(index.column());
            result = key;
        }
    }
    else if (role == Qt::DecorationRole)
    {
        if (index.row() == 1)
        {
            QString key = this->mContainer->keys().at(index.column());
            const QImage *original = this->mContainer->image(key);

            QImage modified = this->modifyImage(original, true);

            QPixmap pixmap = QPixmap::fromImage(modified);
            result = pixmap;
        }
    }
    else if (role == Qt::SizeHintRole)
    {
        if (index.row() == 1)
        {
            QString key = this->mContainer->keys().at(index.column());
            const QImage *original = this->mContainer->image(key);

            QImage modified = this->modifyImage(original, true);

            QSize size = modified.size();
            /*if (size.height() > 30)
            {
                float m = ((float)size.height()) / 30.0f;
                int w = (float)size.width() / m;
                int h = (float)size.height() / m;
                size = QSize(w, h);
            }*/
            result = size;
        }
    }
    else if (role == CropResultRole)
    {
        QString key = this->mContainer->keys().at(index.column());
        const QImage *original = this->mContainer->image(key);

        QImage modified = this->modifyImage(original, false);

        result = modified;
    }
    return result;
}
//-----------------------------------------------------------------------------
QModelIndex ResizeModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex ResizeModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
void ResizeModel::callReset()
{
    this->reset();
}
//-----------------------------------------------------------------------------
void ResizeModel::setCrop(int left, int top, int right, int bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;

    emit this->dataChanged(
                this->index(0, 0),
                this->index(1, this->mContainer->count() - 1));
}
//-----------------------------------------------------------------------------
void ResizeModel::setScale(int scale)
{
    if (scale > 0)
    {
        this->mScale = scale;

        emit this->dataChanged(
                    this->index(0, 0),
                    this->index(1, this->mContainer->count() - 1));
    }
}
//-----------------------------------------------------------------------------
QImage ResizeModel::modifyImage(const QImage *source, bool preview) const
{
    if (preview)
    {
        QImage modified = BitmapHelper::crop(source, this->mLeft, this->mTop, this->mRight, this->mBottom, BitmapEditorOptions::color2());
        QImage scaled = BitmapHelper::scale(&modified, this->mScale);
        QImage grids = BitmapHelper::drawGrid(&scaled, this->mScale);
        return grids;
    }
    else
    {
        QImage modified = BitmapHelper::crop(source, this->mLeft, this->mTop, this->mRight, this->mBottom, BitmapEditorOptions::color2());
        return modified;
    }
}
//-----------------------------------------------------------------------------
