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

#ifndef IIMAGEEDITORTOOL_H
#define IIMAGEEDITORTOOL_H
//-----------------------------------------------------------------------------
class QString;
class QIcon;
class QAction;
class QWidget;
class QMouseEvent;
class QImage;
template <class T1> class QList;
//-----------------------------------------------------------------------------
namespace ImageEditor
{
//-----------------------------------------------------------------------------
class IImageEditorTool
{
public:
    virtual ~IImageEditorTool() { }

    virtual const QString title() const = 0;
    virtual const QString tooltip() const = 0;
    virtual const QIcon *icon() const = 0;
    virtual const QList<QAction *> *actions() const = 0;
    virtual const QList<QWidget *> *widgets() const = 0;

public slots:
    virtual bool processMouse(QMouseEvent *event,
                              const QImage *imageOriginal,
                              bool inRect) = 0;

signals:
    virtual void started(const QImage *value) = 0;
    virtual void processing(const QImage *value) = 0;
    virtual void completed(const QImage *value, bool changed) = 0;
};
//-----------------------------------------------------------------------------
} // end of namespace
Q_DECLARE_INTERFACE (ImageEditor::IImageEditorTool,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // IIMAGEEDITORTOOL_H

