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

#include "fontoptions.h"

#include <QStringList>
#include <QSettings>
#include <QTextCodec>
#include <QtXml>
#include <QDomDocument>

const QString FontOptions::GroupName = QString("font");
const QString FontOptions::FieldBom = QString("bom");
const QString FontOptions::FieldSortOrder = QString("sortOrder");
const QString FontOptions::FieldCodec = QString("codec");

FontOptions::FontOptions(QObject *parent) :
    QObject(parent)
{
    this->mBom = false;
    this->mEncoding = FontOptions::encodings().at(0);
    this->mSortOrder = CharactersSortAscending;
}

bool FontOptions::bom() const
{
    return this->mBom;
}

const QString &FontOptions::encoding() const
{
    return this->mEncoding;
}

CharactersSortOrder FontOptions::sortOrder() const
{
    return this->mSortOrder;
}

void FontOptions::setBom(bool value)
{
    this->mBom = value;

    emit this->changed();
}

void FontOptions::setEncoding(const QString &value)
{
    if (FontOptions::encodings().contains(value))
    {
        this->mEncoding = value;

        emit this->changed();
    }
}

void FontOptions::setSortOrder(CharactersSortOrder value)
{
    this->mSortOrder = value;

    emit this->changed();
}

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
    else if (version == 2)
    {
        settings->beginGroup(FontOptions::GroupName);

        quint32 uBom;
        quint32 uSortOrder;
        QString sEncoding;

        uBom = settings->value(FontOptions::FieldBom, int(0)).toInt(&result);

        if (result)
            uSortOrder = settings->value(FontOptions::FieldSortOrder, int(CharactersSortNone)).toInt(&result);

        if (result)
            sEncoding = settings->value(FontOptions::FieldCodec, QString("UTF-8")).toString();

        if (result)
        {
            this->setBom((bool)uBom);
            this->setEncoding(sEncoding);
            this->setSortOrder((CharactersSortOrder)uSortOrder);
        }

        settings->endGroup();
    }

    return result;
}

bool FontOptions::loadXmlElement(QDomElement element)
{
    bool result = false;

    QDomNode nodeSett = element.firstChild();

    while (!nodeSett.isNull()) {
        QDomElement e = nodeSett.toElement();

        if (e.tagName() == FontOptions::GroupName) {
            break;
        }

        nodeSett = nodeSett.nextSibling();
    }

    if (nodeSett.isNull()) {
        return result;
    }

    quint32 uBom = 0;
    quint32 uSortOrder = int(CharactersSortNone);
    QString sEncoding = "UTF-8";

    QDomNode nodeValue = nodeSett.firstChild();

    while (!nodeValue.isNull()) {
        QDomElement e = nodeValue.toElement();

        if (!e.isNull()) {
            if (e.tagName() == FontOptions::FieldBom) {
                QString str = e.text();
                uBom = str.toUInt(&result);
            }

            if (e.tagName() == FontOptions::FieldSortOrder) {
                QString str = e.text();
                uSortOrder = str.toUInt(&result);
            }

            if (e.tagName() == FontOptions::FieldCodec) {
                sEncoding = e.text();
            }

            if (!result) {
                break;
            }
        }

        nodeValue = nodeValue.nextSibling();
    }

    if (result)
    {
        this->setBom((bool)uBom);
        this->setEncoding(sEncoding);
        this->setSortOrder((CharactersSortOrder)uSortOrder);
    }

    return result;
}

void FontOptions::save(QSettings *settings)
{
    settings->beginGroup(FontOptions::GroupName);

    settings->setValue(FontOptions::FieldBom, QString("%1").arg((int)this->bom()));
    settings->setValue(FontOptions::FieldSortOrder, QString("%1").arg((int)this->sortOrder()));
    settings->setValue(FontOptions::FieldCodec,  this->encoding());

    settings->endGroup();
}

void FontOptions::saveXmlElement(QDomElement element)
{
    QDomElement nodeFont = element.ownerDocument().createElement(FontOptions::GroupName);
    element.appendChild(nodeFont);

    QDomElement nodeBom = element.ownerDocument().createElement(FontOptions::FieldBom);
    nodeFont.appendChild(nodeBom);
    nodeBom.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->bom())));

    QDomElement nodeSortOrder = element.ownerDocument().createElement(FontOptions::FieldSortOrder);
    nodeFont.appendChild(nodeSortOrder);
    nodeSortOrder.appendChild(element.ownerDocument().createTextNode(QString("%1").arg((int)this->sortOrder())));

    QDomElement nodeCodec = element.ownerDocument().createElement(FontOptions::FieldCodec);
    nodeFont.appendChild(nodeCodec);
    nodeCodec.appendChild(element.ownerDocument().createTextNode(this->encoding()));
}

const QStringList &FontOptions::encodings()
{
    static QStringList result;

    if (result.isEmpty())
    {
        QList<QByteArray> codecs = QTextCodec::availableCodecs();
        for (int i = 0; i < codecs.length(); i++)
        {
            result.append(QString(codecs.at(i)));
        }
        result.sort();
    }
    return result;
}

