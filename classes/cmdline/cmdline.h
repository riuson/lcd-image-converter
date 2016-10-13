/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2014 riuson
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

#include "qt-version-check.h"

#if QT_VERSION_COMBINED >= VERSION_COMBINE(5, 2, 0)

#ifndef CMDLINE_H
#define CMDLINE_H

#include <QObject>

class QString;
class QStringList;
class QCommandLineParser;

namespace CommandLine {

class ModeParserBase;

class CmdLine : public QObject
{
    Q_OBJECT
public:
    explicit CmdLine(const QStringList &arguments, QObject *parent = 0);
    virtual ~CmdLine();

    bool needProcess() const;
    int process();
private:
    QCommandLineParser *mParser;
    const QStringList *mArguments;

    CommandLine::ModeParserBase *createMode(const QString &name, QCommandLineParser *parser);
};

}

#endif // CMDLINE_H

#endif // QT_VERSION
