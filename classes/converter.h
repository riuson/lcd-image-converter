/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
 * Copyright (C) 2010 riuson
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

#ifndef CONVERTER_H
#define CONVERTER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QMap>

class IDocument;
class IDataContainer;
class ConversionMatrix;
//-----------------------------------------------------------------------------
class Converter : public QObject
{
    Q_OBJECT
public:
    Converter(QObject *parent);
    ~Converter();

    QString name();

    QString convert(IDocument *document, const QString &templateFile, QMap<QString, QString> &tags);
private:
    QMap <QString, QObject *> mConverters;
    QString mSelectedPresetName;
    ConversionMatrix *mMatrix;

    void parse(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseBlocks(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseImagesTable(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    void parseSimple(const QString &templateString, QString &resultString, QMap<QString, QString> &tags, IDocument *doc);
    QString hexCode(const QChar &ch, const QString &encoding, bool bom);
    void addOrderInfo(QMap<QString, QString> &tags);
    void addPreprocessInfo(QMap<QString, QString> &tags);
};
//-----------------------------------------------------------------------------
#endif // CONVERTER_H
