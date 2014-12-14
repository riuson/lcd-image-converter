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

#ifndef ACTIONFILEHANDLERS_H
#define ACTIONFILEHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class QImage;
class IMainWindow;
class IDocument;
//-----------------------------------------------------------------------------
class ActionFileHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionFileHandlers(QObject *parent = 0);

signals:
    void rememberFilename(const QString &filename);
    void closeRequest(QWidget *tab);
    void tabChanged(QWidget *w);
    void tabCreated(QWidget *w);

public slots:
    void newImage_triggered();
    void newFont_triggered();
    void open_triggered();
    void rename_triggered();
    void save_triggered();
    void saveAs_triggered();
    void close_triggered();
    void convert_triggered();
    void convertAll_triggered();
    void openFiles(const QStringList &filenames);
    void openFile(const QString &filename);
    void openImage(QImage *image, const QString &documentName);

private:
    void openBinaryImage(const QStringList &filenames);
    void openImage(const QStringList &filenames);
    void openFont(const QStringList &filenames);
    void convertDocument(IDocument *document, bool request);

private slots:
    void documentChanged();
};
//-----------------------------------------------------------------------------
#endif // ACTIONFILEHANDLERS_H
