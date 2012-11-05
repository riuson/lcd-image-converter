/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
class QStringList;
class QSettings;
//-----------------------------------------------------------------------------
class FontOptions : public QObject
{
    Q_OBJECT
public:
    explicit FontOptions(QObject *parent = 0);

    bool bom() const;
    const QString &encoding() const;

    void setBom(bool value);
    void setEncoding(const QString &value);

    bool load(QSettings *settings, int version);
    void save(QSettings *settings);

    static const QStringList &encodings();

private:
    bool mBom;
    QString mEncoding;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTOPTIONS_H
