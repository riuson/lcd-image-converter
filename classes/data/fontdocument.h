/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef FONTDOCUMENT_H
#define FONTDOCUMENT_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QFont>

#include "idocument.h"
//-----------------------------------------------------------------------------
template <class T1, class T2> class QMap;
class Preset;
class ParsedImageData;
class Tags;
struct tFontParameters;
//-----------------------------------------------------------------------------
class FontDocument : public QObject, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit FontDocument(QObject *parent = 0);
    ~FontDocument();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    bool changed() const;
    QString documentFilename() const;
    QString documentName() const;
    void setDocumentName(const QString &value);
    QString outputFilename() const;
    void setOutputFilename(const QString &value);
    DataContainer *dataContainer() const;
    QString convert(Preset *preset);

    void beginChanges();
    void endChanges(bool suppress);
    bool canUndo();
    bool canRedo();
    void undo();
    void redo();

    void fontCharacters(QString *chars,
                        tFontParameters *parameters);
    void setFontCharacters(const QString &chars,
                           const tFontParameters &parameters);

private:
    DataContainer *mContainer;
    int mNestedChangesCounter;

    void setDocumentFilename(const QString &value);

    QFont usedFont() const;
    void setUsedFont(const QFont &value);

    QString usedStyle() const;
    void setUsedStyle(const QString &value);

    bool monospaced() const;
    void setMonospaced(const bool value);

    bool antialiasing() const;
    void setAntialiasing(const bool value);

    int ascent() const;
    void setAscent(int value);

    int descent() const;
    void setDescent(int value);

    void prepareImages(Preset *preset, const QStringList &orderedKeys, QMap<QString, ParsedImageData*> *images, const Tags &tags) const;
    QString hexCode(const QString &key, const QString &encoding, bool bom) const;
    const QStringList sortKeysWithEncoding(const QStringList &keys, Preset *preset) const;

private slots:
    void mon_container_dataChanged(bool historyStateMoved);

signals:
    void documentChanged();
};

//-----------------------------------------------------------------------------
#endif // FONTDOCUMENT_H
