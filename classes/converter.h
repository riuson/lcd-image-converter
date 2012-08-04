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
#include "iconverter.h"

#include <QObject>
#include <QMap>

class IDocument;
class IDataContainer;
//-----------------------------------------------------------------------------
class Converter : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    Converter(QObject *parent);
    ~Converter();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);
    QString dataToString(const BitmapData &data);

    bool swapBytes();
    DataLength length();
    bool mirror();
    bool pack();
    DataAlign align();

    void setSwapBytes(bool value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setAlign(DataAlign value);

    QStringList names() const ;
    QWidget *widgetSetup();
    IConverter *conv(const QString &name);
    void selectConv(const QString &name);
    QString convert(IDocument *document, const QString &templateFile, QMap<QString, QString> &tags);
    int transform();
    void setTransform(int value);
private:
    QMap <QString, QObject *> mConverters;
    QString mSelectedConverterName;
    int mPreprocessTransform;

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
