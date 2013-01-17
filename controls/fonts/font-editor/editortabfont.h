/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
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

#ifndef EDITORTABFONT_H
#define EDITORTABFONT_H
//-----------------------------------------------------------------------------
#include <QWidget>

#include "idocument.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabFont;
}
//-----------------------------------------------------------------------------
class DataContainer;
class WidgetBitmapEditor;
class QSplitter;
class FontCharactersModel;
class QItemSelection;
class StatusData;
//-----------------------------------------------------------------------------
class EditorTabFont : public QWidget, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit EditorTabFont(QWidget *parent = 0);
    ~EditorTabFont();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    bool changed() const;
    void setChanged(bool value);
    QString fileName() const;
    QString documentName() const;
    void setDocumentName(const QString &value);
    DataContainer *dataContainer();
    const QImage *image() const;
    void setImage(const QImage *value);
    void convert(bool request);
    StatusData *statusData() const;

    void beginChanges();
    void endChanges();
    bool canUndo();
    bool canRedo();
    void undo();
    void redo();

    void setFontCharacters(const QString &chars,
                           const QString &fontFamily,
                           const QString &_style,
                           const int _size,
                           const bool _monospaced,
                           const bool _antialiasing);
    void fontCharacters(QString *chars,
                        QString *fontFamily,
                        QString *_style,
                        int *_size,
                        bool *_monospaced,
                        bool *_antialiasing);

    const QString selectedCharacters() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabFont *ui;
    WidgetBitmapEditor *mEditor;
    DataContainer *mContainer;
    QSplitter *mSplitter;
    FontCharactersModel *mModel;
    StatusData *mStatusData;

    QFont mTableFont;
    QString mSelectedKey;

    void setFileName(const QString &value);
    QString convertedFileName() const;
    void setConvertedFileName(const QString &value);
    QFont usedFont() const;
    void setUsedFont(const QFont &value);
    QString usedStyle() const;
    void setUsedStyle(const QString &value);
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
    void updateTableFont();

private slots:
    void mon_container_imageChanged(const QString &key);
    void mon_editor_imageChanged();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
signals:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
    void statusChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
