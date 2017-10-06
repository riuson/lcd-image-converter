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

#ifndef FONTHELPER_H
#define FONTHELPER_H

#include <QObject>

#include <QImage>
#include <QSize>

class DataContainer;

class FontHelper : public QObject
{
  Q_OBJECT
public:
  static QImage drawString(const DataContainer *data, const QString &value);
  static QString escapeControlChars(const QString &value);
  static QString unescapeControlChars(const QString &value);
  static QSize getCharacterSize(const QFontMetrics &metrics, QChar value);
  static QImage drawCharacter(const QChar value,
                              const QFont &font,
                              const QColor &foreground,
                              const QColor &background,
                              const int width,
                              const int height,
                              const bool antialiasing);
signals:

public slots:

};

#endif // FONTHELPER_H
