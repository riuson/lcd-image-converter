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
class FontContainer;
class WidgetBitmapEditor;
class QSplitter;
class FontCharactersModel;
class QItemSelection;
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
    bool changed();
    void setChanged(bool value);
    QString fileName();
    QString documentName();
    void setDocumentName(const QString &value);
    IDataContainer *dataContainer();
    WidgetBitmapEditor *editor();

    void setFontCharacters(const QString &chars,
                           const QString &fontFamily,
                           const QString &style,
                           const int size,
                           const bool monospaced,
                           const bool antialiasing);
    void fontCharacters(QString *chars,
                        QString *fontFamily,
                        QString *style,
                        int *size,
                        bool *monospaced,
                        bool *antialiasing);

    const QString selectedCharacters() const;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabFont *ui;
    WidgetBitmapEditor *mEditor;
    FontContainer *mContainer;
    QSplitter *mSplitter;
    FontCharactersModel *mModel;

    QString mFileName;
    QString mDocumentName;
    bool mDataChanged;
    QFont mFont;

    //QString mCharacters;
    QString mStyle;
    bool mMonospaced;
    bool mAntialiasing;

    QImage drawCharacter(const QChar value,
                         const QFont &font,
                         const QColor &foreground,
                         const QColor &background,
                         const int width,
                         const int height,
                         const bool antialiasing);

private slots:
    void mon_editor_dataChanged();
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
signals:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
