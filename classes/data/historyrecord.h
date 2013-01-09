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

#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QMap>
//-----------------------------------------------------------------------------
class HistoryRecord : public QObject
{
    Q_OBJECT
public:
    explicit HistoryRecord(
            const QMap<QString, QImage *> *_images,
            const QMap<QString, QVariant> *_info,
            QObject *parent = 0);
    ~HistoryRecord();

    const QMap<QString, QImage *> *images() const;
    const QMap<QString, QVariant> *info() const;

private:
    QMap<QString, QImage *> mImageMap;
    QMap<QString, QVariant> mInfoMap;

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // HISTORYRECORD_H
