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

#ifndef TOOLCOLOR_H
#define TOOLCOLOR_H

#include <QObject>
#include <QImage>
#include <QColor>
#include "iimageeditortool.h"

class QIcon;
class QAction;

namespace ImageEditor
{
class IImageEditorParams;

class ToolColor : public QObject, public IImageEditorTool
{
    Q_OBJECT
    Q_INTERFACES(ImageEditor::IImageEditorTool)

public:
    explicit ToolColor(IImageEditorParams *parameters, QObject *parent = 0);
    ~ToolColor();

    const QString title() const;
    const QString tooltip() const;
    const QIcon *icon() const;
    const QList<QAction *> *actions() const;
    const QList<QWidget *> *widgets() const;

    const QColor foreColor() const;
    const QColor backColor() const;

public slots:
    bool processMouse(QMouseEvent *event,
                      const QImage *imageOriginal,
                      bool inRect);

signals:
    void started(const QImage *value);
    void processing(const QImage *value);
    void completed(const QImage *value, bool changed);

private:
    IImageEditorParams *mParameters;
    QIcon *mIcon;
    QList<QAction *> *mActions;
    QList<QWidget *> *mWidgets;
    QColor mForeColor;
    QColor mBackColor;
    QAction *mActionForeColor;
    QAction *mActionBackColor;
    QAction *mActionSwapColors;

    void initializeWidgets();
    void loadSettings();
    void saveSettings() const;
    void updateColorIcons();

private slots:
    void on_buttonForeColor_triggered();
    void on_buttonBackColor_triggered();
    void on_buttonSwapColors_triggered();
};

} // end of namespace

#endif // TOOLCOLOR_H
