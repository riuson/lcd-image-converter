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

#include "fonthelper.h"
//-----------------------------------------------------------------------------
#include <QPixmap>
#include <QPainter>
#include <QRegExp>
#include "datacontainer.h"
//-----------------------------------------------------------------------------
FontHelper::FontHelper(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
QImage FontHelper::drawString(const DataContainer *data, const QString &value)
{
    int width = 0, height = 0;
    QImage::Format format = QImage::Format_ARGB32;
    // find summary preview image size
    for (int i = 0; i < value.length(); i++)
    {
        QChar ch = value.at(i);
        if (data->keys().contains(ch))
        {
            const QImage *image = data->image(ch);

            format = image->format();
            width += image->width();
            height = qMax(height, image->height());
        }
    }

    // create preview image
    QImage preview = QImage(width, height, format);
    QPixmap previewPixmap = QPixmap::fromImage(preview);
    QPainter painter(&previewPixmap);

    // place characters on image
    int x = 0;
    for (int i = 0; i < value.length(); i++)
    {
        QChar ch = value.at(i);
        if (data->keys().contains(ch))
        {
            const QImage *image = data->image(ch);
            painter.drawImage(x, 0, *image);

            x += image->width();
        }
    }

    return previewPixmap.toImage();
}
//-----------------------------------------------------------------------------
QString FontHelper::escapeControlChars(const QString &value)
{
    QString result;

    for (int i = 0; i < value.length(); i++)
    {
        QChar c = value.at(i);

        if (c.isNull() || !c.isPrint())
        {
            quint16 code = c.unicode();
            result.append(QString("\\x%1").arg(code, 4, 16, QChar('0')));
        }
        else if (c == QChar('@'))
        {
            result.append(QString("\\x0040"));
        }
        else if (c == QChar(QChar::Nbsp))
        {
            result.append(QString("\\x00a0"));
        }
        else
        {
            result.append(c);
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
QString FontHelper::unescapeControlChars(const QString &value)
{
    QRegExp reg("\\\\x([abcdef\\d]{4})", Qt::CaseInsensitive);
    QString result = value;
    int index = -1;
    do
    {
        if ((index = result.indexOf(reg)) >= 0)
        {
            QString cap0 = reg.cap(0);
            QString cap1 = reg.cap(1);
            bool ok;
            int code = cap1.toInt(&ok, 16);
            if (ok)
            {
                result = result.replace(cap0, QString(QChar(code)));
            }
        }

    } while(index >= 0);
    return result;
}
//-----------------------------------------------------------------------------
