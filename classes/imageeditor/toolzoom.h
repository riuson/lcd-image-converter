/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2015 riuson
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

#ifndef TOOLZOOM_H
#define TOOLZOOM_H
//-----------------------------------------------------------------------------
#include <QObject>
#include "iimageeditortool.h"

class QIcon;
class QSpinBox;
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
class ToolZoom : public QObject, public IImageEditorTool
{
    Q_OBJECT
    Q_INTERFACES(ImageEditor::IImageEditorTool)

public:
    explicit ToolZoom(QObject *parent = 0);
    ~ToolZoom();

    const QString title() const;
    const QString tooltip() const;
    const QIcon *icon() const;
    const QList<QAction *> *actions() const;
    const QList<QWidget *> *widgets() const;

public slots:
    void mousePress(const QMouseEvent *event);
    void mouseMove(const QMouseEvent *event);
    void mouseRelease(const QMouseEvent *event);
    void changeZoom(int value);

signals:
    void started();
    void completed();
    void zoomChanged(int value);

private:
    QIcon *mIcon;
    QSpinBox *mSpinBoxZoom;
    QList<QAction *> *mActions;
    QList<QWidget *> *mWidgets;
    int mZoom;


    void initializeWidgets();

private slots:
    void on_spinBoxZoom_valueChanged(int value);
};
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
#endif // TOOLZOOM_H
