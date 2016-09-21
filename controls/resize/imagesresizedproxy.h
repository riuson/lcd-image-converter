/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2016 riuson
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

#ifndef IMAGESRESIZEDPROXY_H
#define IMAGESRESIZEDPROXY_H

#include <QSortFilterProxyModel>

class ImagesResizedProxy : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ImagesResizedProxy(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setCrop(int left, int top, int right, int bottom);

private:
    int mLeft;
    int mRight;
    int mTop;
    int mBottom;
};

#endif // IMAGESRESIZEDPROXY_H
