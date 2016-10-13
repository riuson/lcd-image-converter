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

#ifndef RECENTLIST_H
#define RECENTLIST_H

#include <QObject>

//class QStringList;
//class QStringListIterator;

class RecentList : public QObject
{
    Q_OBJECT
public:
    explicit RecentList(QObject *parent = 0);
    virtual ~RecentList();
    void add(const QString &filename);
    enum { MaxRecentFiles = 10 };
    const QStringList * files() const;

private:
    QStringList *mFiles;
signals:
    void listChanged();
public slots:
    
};

#endif // RECENTLIST_H
