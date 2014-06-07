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
class Preset;
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
    DataContainer *dataContainer();
    QString convert(Preset *preset);

    void beginChanges();
    void endChanges(bool suppress);
    bool canUndo();
    bool canRedo();
    void undo();
    void redo();

    void fontCharacters(QString *chars,
                        QString *fontFamily,
                        bool *_bold,
                        bool *_italic,
                        int *_size,
                        bool *_monospaced,
                        bool *_antialiasing);
    void setFontCharacters(const QString &chars,
                           const QString &fontFamily,
                           const bool &_bold,
                           const bool &_italic,
                           const int _size,
                           const bool _monospaced,
                           const bool _antialiasing);

private:
    DataContainer *mContainer;
    int mNestedChangesCounter;

    void setDocumentFilename(const QString &value);

    QFont usedFont() const;
    void setUsedFont(const QFont &value);

    bool bold() const;
    void setBold(bool value);

    bool italic() const;
    void setItalic(bool value);

    bool monospaced() const;
    void setMonospaced(const bool value);

    bool antialiasing() const;
    void setAntialiasing(const bool value);

    QImage drawCharacter(const QChar value,
                         const QFont &font,
                         const QColor &foreground,
                         const QColor &background,
                         const int width,
                         const int height,
                         const bool antialiasing);

private slots:
    void mon_container_dataChanged(bool historyStateMoved);

signals:
    void documentChanged();
};
//-----------------------------------------------------------------------------
#endif // FONTDOCUMENT_H
