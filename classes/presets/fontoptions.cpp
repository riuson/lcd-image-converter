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

#include "fontoptions.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QSettings>
//-----------------------------------------------------------------------------
FontOptions::FontOptions(QObject *parent) :
    QObject(parent)
{
    this->mBom = false;
    this->mEncoding = FontOptions::encodings().at(0);
}
//-----------------------------------------------------------------------------
bool FontOptions::bom() const
{
    return this->mBom;
}
//-----------------------------------------------------------------------------
const QString &FontOptions::encoding() const
{
    return this->mEncoding;
}
//-----------------------------------------------------------------------------
void FontOptions::setBom(bool value)
{
    this->mBom = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void FontOptions::setEncoding(const QString &value)
{
    if (FontOptions::encodings().contains(value))
    {
        this->mEncoding = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
bool FontOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        quint32 uBom;
        QString sEncoding;

        uBom = settings->value("fontUseBom", int(0)).toInt(&result);

        if (result)
            sEncoding = settings->value("fontCodec", QString("UTF-8")).toString();

        if (result)
        {
            this->setBom((bool)uBom);
            this->setEncoding(sEncoding);
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
void FontOptions::save(QSettings *settings)
{
    settings->setValue("fontUseBom", QString("%1").arg((int)this->bom()));
    settings->setValue("fontCodec",  this->encoding());
}
//-----------------------------------------------------------------------------
const QStringList &FontOptions::encodings()
{
    static const QStringList result =
            QStringList() << "UTF-8"
                          << "UTF-16"
                          << "UTF-16BE"
                          << "UTF-16LE"
                          << "UTF-32"
                          << "UTF-32BE"
                          << "UTF-32LE"
                          << "Windows-1250"
                          << "Windows-1251"
                          << "Windows-1252"
                          << "Windows-1253"
                          << "Windows-1254"
                          << "Windows-1255"
                          << "Windows-1256"
                          << "Windows-1257"
                          << "Windows-1258";

    return result;
}
//-----------------------------------------------------------------------------
