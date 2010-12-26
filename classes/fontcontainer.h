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

#ifndef FONTCONTAINER_H
#define FONTCONTAINER_H

#include <QObject>

#include <QMap>
#include <QString>

#include "idatacontainer.h"
//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------
class FontContainer : public QObject, public IDataContainer
{
    Q_OBJECT
    Q_INTERFACES(IDataContainer)
public:
    explicit FontContainer(QObject *parent = 0);
    ~FontContainer();
private:
    QMap<QString, QImage *> mImageMap;
    QImage *mDefaultImage;
public:
    QImage *image(const QString &key);
    void setImage(const QString &key, QImage *image);
    void transform(const QString &key, int code);
    int count();
    QStringList keys() const;

    void clear();
    void remove(const QString &key);
signals:
    void imageChanged(const QString &key);
public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTCONTAINER_H
