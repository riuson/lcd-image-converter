/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QObject>

#include <QMap>
#include <QString>
#include <QStringList>

//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------
class DataContainer : public QObject
{
    Q_OBJECT
public:
    explicit DataContainer(QObject *parent = 0);
    virtual ~DataContainer();

    const QImage *image(const QString &key) const;
    void setImage(const QString &key, const QImage *image);

    void clear();
    int count() const;
    QStringList keys() const;
    void remove(const QString &key);

private:
    QMap<QString, QImage *> mImageMap;
    QImage *mDefaultImage;

signals:
    void imageChanged(const QString &key);

public slots:

};
//-----------------------------------------------------------------------------
#endif // DATACONTAINER_H
