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
    bool changed();
    QString fileName();
    QString documentName();
    void setDocumentName(const QString &value);
    DataContainer *dataContainer();
    WidgetBitmapEditor *editor();
    void convert(bool request);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabImage *ui;
    WidgetBitmapEditor *mEditor;
    DataContainer *mContainer;

    QString mFileName;
    QString mConvertedFileName;
    QString mDocumentName;
    bool mDataChanged;

    static const QString DefaultKey;

private slots:
    void mon_editor_imageChanged();
signals:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
};
//-----------------------------------------------------------------------------
#endif // EDITORTABIMAGE_H
