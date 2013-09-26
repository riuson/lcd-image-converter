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

#ifndef MATRIXPREVIEWMODEL_H
#define MATRIXPREVIEWMODEL_H
//-----------------------------------------------------------------------------
#include <QAbstractItemModel>
//-----------------------------------------------------------------------------
#include <QVariant>
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class Preset;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class MatrixPreviewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum RowType
    {
        Source,
        Operation,
        MaskUsed,
        MaskAnd,
        MaskOr,
        MaskFill,
        Result,
        ResultPacked
    };

    explicit MatrixPreviewModel(Preset *preset, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    RowType rowType(int row) const;

private:
    Preset *mPreset;

    enum ColorType
    {
        Empty,
        // for colors
        Alpha,
        Red,
        Green,
        Blue,
        // for monochrome
        BlackOrWhite,
        //for grayscale
        Gray
    };

    void getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const;
    void resultToSourceBit(int bitIndex, QVariant *name, QVariant *color) const;
    void resultPackedToSourceBit(int bitIndex, QVariant *name, QVariant *color) const;
    void sourceBitProperties(int bitIndex, QVariant *name, QVariant *color) const;

signals:

public slots:
    void callReset();
};
//-----------------------------------------------------------------------------
#endif // MATRIXPREVIEWMODEL_H
