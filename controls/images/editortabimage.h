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

#ifndef EDITORTABIMAGE_H
#define EDITORTABIMAGE_H
//-----------------------------------------------------------------------------
#include <QWidget>

#include "idocument.h"
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabImage;
}
//-----------------------------------------------------------------------------
class QImage;
class WidgetBitmapEditor;
class DataContainer;
class StatusData;
//-----------------------------------------------------------------------------
class EditorTabImage : public QWidget, public IDocument
{
    Q_OBJECT
    Q_INTERFACES(IDocument)

public:
    explicit EditorTabImage(QWidget *parent = 0);
    ~EditorTabImage();

    bool load(const QString &fileName);
    bool save(const QString &fileName);
    void setChanged(bool value);
    bool changed() const;
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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabImage *ui;
    WidgetBitmapEditor *mEditor;
    DataContainer *mContainer;
    StatusData *mStatusData;

    static const QString DefaultKey;

    void setFileName(const QString &value);
    QString convertedFileName() const;
    void setConvertedFileName(const QString &value);

private slots:
    void mon_container_imageChanged(const QString &key);
    void mon_editor_imageChanged();
signals:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
    void statusChanged();
};
//-----------------------------------------------------------------------------
#endif // EDITORTABIMAGE_H
