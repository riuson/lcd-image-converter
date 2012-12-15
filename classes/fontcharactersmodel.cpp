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

#include "fontcharactersmodel.h"
//-----------------------------------------------------------------------------
#include <QPixmap>
#include "fontcontainer.h"
//-----------------------------------------------------------------------------
FontCharactersModel::FontCharactersModel(FontContainer *container, QObject *parent) :
    QAbstractItemModel(parent)
{
    this->mContainer = container;
}
//-----------------------------------------------------------------------------
int FontCharactersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->mContainer->count();
}
//-----------------------------------------------------------------------------
int FontCharactersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
//-----------------------------------------------------------------------------
QVariant FontCharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
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
QVariant FontCharactersModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();
    if (role == Qt::DisplayRole && index.isValid())
    {
        if (index.column() == 0)
        {
            QString key = this->mContainer->keys().at(index.row());
            result = key;
        }
    }
    else if (role == Qt::DecorationRole && index.isValid())
    {
        if (index.column() == 1)
        {
            QString key = this->mContainer->keys().at(index.row());
            QImage im = *this->mContainer->image(key);
            QSize size = im.size();
            if (size.height() > 30)
            {
                float m = ((float)size.height()) / 30.0f;
                int w = (float)size.width() / m;
                int h = (float)size.height() / m;
                im = im.scaled(w, h);
            }
            QPixmap pixmap = QPixmap::fromImage(im);
            result = pixmap;
        }
    }
    else if (role == Qt::SizeHintRole && index.isValid())
    {
        if (index.column() == 1)
        {
            QString key = this->mContainer->keys().at(index.row());
            QImage im = *this->mContainer->image(key);
            QSize size = im.size();
            if (size.height() > 30)
            {
                float m = ((float)size.height()) / 30.0f;
                int w = (float)size.width() / m;
                int h = (float)size.height() / m;
                size = QSize(w, h);
            }
            result = size;
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QModelIndex FontCharactersModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex FontCharactersModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------
void FontCharactersModel::callReset()
{
    this->reset();
}
//-----------------------------------------------------------------------------
