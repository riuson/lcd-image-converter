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

#include "ieditor.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabFont;
}
//-----------------------------------------------------------------------------
class DataContainer;
class WidgetBitmapEditor;
class QSplitter;
class ImagesModel;
class QItemSelection;
class QModelIndex;
class StatusData;
class FontDocument;
class IDocument;
//-----------------------------------------------------------------------------
class EditorTabFont : public QWidget, public IEditor
{
    Q_OBJECT
    Q_INTERFACES(IEditor)

public:
    explicit EditorTabFont(QWidget *parent = 0);
    ~EditorTabFont();

    IDocument *document() const;
    QStringList selectedKeys() const;
    StatusData *statusData() const;
    EditorType type() const;

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

protected:
    void changeEvent(QEvent *e);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::EditorTabFont *ui;
    WidgetBitmapEditor *mEditor;
    FontDocument *mDocument;
    QSplitter *mSplitter;
    ImagesModel *mModel;
    StatusData *mStatusData;

    QFont mTableFont;

    void initStatusData();
    void updateStatus();

    QString currentKey() const;
    void updateSelectedImage();

private slots:
    void mon_documentChanged();
    void mon_editor_imageChanged();
    void mon_editor_mouseMove(QPoint point);
    void mon_editor_scaleChanged(int scale);
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void updateTableFont();
    void resizeToContents();
signals:
    void documentChanged();
    void statusChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
