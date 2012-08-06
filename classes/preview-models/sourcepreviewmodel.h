/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#ifndef SOURCEPREVIEWMODEL_H
#define SOURCEPREVIEWMODEL_H
//-----------------------------------------------------------------------------
#include <QAbstractItemModel>
#include <QVariant>

#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
class SourcePreviewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SourcePreviewModel(QList<quint32> *matrix, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;

private:
    QList<quint32> *mMatrix;

    enum ColorType
    {
        Empty,
        // for colors
        Red,
        Green,
        Blue,
        // for monochrome
        BlackOrWhite,
        //for grayscale
        Gray
    };

    void getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const;

signals:
    
public slots:
    
};
//-----------------------------------------------------------------------------
#endif // SOURCEPREVIEWMODEL_H
