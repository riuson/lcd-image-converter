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

#ifndef FONTOPTIONS_H
#define FONTOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class QStringList;
class QSettings;
class QDomElement;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class FontOptions : public QObject
{
    Q_OBJECT
public:
    explicit FontOptions(QObject *parent = 0);

    bool bom() const;
    const QString &encoding() const;
    CharactersSortOrder sortOrder() const;

    void setBom(bool value);
    void setEncoding(const QString &value);
    void setSortOrder(CharactersSortOrder value);

    bool load(QSettings *settings, int version);
    void save(QSettings *settings);
    void saveXmlElement(QDomElement *element);

    static const QStringList &encodings();

private:
    static const QString GroupName;
    static const QString FieldBom;
    static const QString FieldSortOrder;
    static const QString FieldCodec;

    bool mBom;
    QString mEncoding;
    CharactersSortOrder mSortOrder;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTOPTIONS_H
