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

#include "recentlist.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QStringListIterator>
#include <QSettings>
#include <QFileInfo>
//-----------------------------------------------------------------------------
RecentList::RecentList(QObject *parent) :
    QObject(parent)
{
    this->mFiles = new QStringList();

    // load from settings
    QSettings sett;
    int size = sett.beginReadArray("recent");
    for (int i = 0; i < size; i++)
    {
        sett.setArrayIndex(i);
        QString filename = sett.value("filename").toString();
        QFileInfo info(filename);
        if (info.exists())
            this->mFiles->append(filename);
    }
    sett.endArray();
}
//-----------------------------------------------------------------------------
RecentList::~RecentList()
{
    // save to settings
    QSettings sett;
    sett.beginWriteArray("recent");
    QStringListIterator recentFilesIterator(*this->mFiles);
    int i = 0;
    while (recentFilesIterator.hasNext())
    {
        sett.setArrayIndex(i++);
        sett.setValue("filename", recentFilesIterator.next());
    }
    sett.endArray();

    delete this->mFiles;
}
//-----------------------------------------------------------------------------
void RecentList::add(const QString &filename)
{
    if (this->mFiles->contains(filename))
        this->mFiles->removeOne(filename);
    this->mFiles->insert(0, filename);

    if (this->mFiles->count() > MaxRecentFiles)
    {
        for (int i = this->mFiles->count() - 1; i >= 10; i--)
            this->mFiles->removeAt(i);
    }

    emit this->listChanged();
}
//-----------------------------------------------------------------------------
const QStringList * RecentList::files() const
{
    return this->mFiles;
}
//-----------------------------------------------------------------------------
