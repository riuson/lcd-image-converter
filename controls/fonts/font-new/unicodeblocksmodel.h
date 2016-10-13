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

#ifndef QUNICODEBLOCKSMODEL_H
#define QUNICODEBLOCKSMODEL_H

#include <QAbstractListModel>
#include <QList>

class UnicodeBlock
{
public:
    UnicodeBlock(const QString &name, quint32 firstCode, quint32 lastCode);
    const QString &name() const;
    quint32 firstCode() const;
    quint32 lastCode() const;

private:
    QString mName;
    quint32 mFirstCode;
    quint32 mLastCode;
};

class UnicodeBlocksModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit UnicodeBlocksModel(QObject *parent = 0);

    enum UnicodeBlocksModelRole
    {
        FirstCodeRole = Qt::UserRole + 1,
        LastCodeRole = Qt::UserRole + 2
    };

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const; // функция доступа к данным
    int rowCount(const QModelIndex &parent) const; // количество элементов в модели

private:
    QList<UnicodeBlock *> mList;

signals:

public slots:

};

#endif // QUNICODEBLOCKSMODEL_H
