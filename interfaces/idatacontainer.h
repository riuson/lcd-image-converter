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

#ifndef IDATACONTAINER_H
#define IDATACONTAINER_H
//-----------------------------------------------------------------------------
#include <QtGlobal>
#include <QString>
#include <QStringList>

class QImage;
//-----------------------------------------------------------------------------
class IDataContainer
{
public:
    virtual QImage *image(const QString &key) = 0;
    virtual void setImage(const QString &key, QImage *image) = 0;
    virtual void transform(const QString &key, int code) = 0;
    virtual int count() = 0;
    virtual QStringList keys() const = 0;
    //virtual void imageChanged() = 0;
};
Q_DECLARE_INTERFACE (IDataContainer,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // IDATACONTAINER_H
