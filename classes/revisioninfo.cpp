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

#include "revisioninfo.h"
//-----------------------------------------------------------------------------
#include <QFile>
#include <QTextStream>
#include <QRegExp>
//-----------------------------------------------------------------------------
// git-commit-info d693078 Sat May 26 22:30:51 2012 +0600
//-----------------------------------------------------------------------------
RevisionInfo::RevisionInfo(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
bool RevisionInfo::getRevisionData(QString *hash, QString *date)
{
    bool result = false;
    // load version info
    QFile file_version(":/text/version_info");
    if (file_version.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file_version);
        QString version = stream.readAll();
        file_version.close();

        // git-commit-info d693078 Sat May 26 22:30:51 2012 +0600

        QString start = "git-commit-info ";
        if (version.startsWith(start, Qt::CaseInsensitive))
        {
            // get hash of commit
            QRegExp reg = QRegExp("[0-9a-f]+", Qt::CaseInsensitive);
            int index;
            if ((index = reg.indexIn(version, start.length())) >= 0)
            {
                *hash = reg.cap();

                // get date
                *date = version.mid(index + (*hash).length() + 1);

                result = true;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
