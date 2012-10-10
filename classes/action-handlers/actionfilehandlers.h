/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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
//-----------------------------------------------------------------------------
class ActionFileHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionFileHandlers(QObject *parent = 0);

signals:
    void rememberFilename(const QString &filename);
    void closeRequest(QWidget *tab);
    void tabChanged(QWidget *w, const QString &documentName, const QString &filename);
    void tabCreated(QWidget *w, const QString &documentName, const QString &filename);

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
    void openFile(const QString &filename);
    void openImage(QImage *image, const QString &documentName);
private slots:
    void documentChanged(bool changed, const QString &documentName, const QString &filename);
};
//-----------------------------------------------------------------------------
#endif // ACTIONFILEHANDLERS_H
