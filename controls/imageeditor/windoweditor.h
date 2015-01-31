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

#ifndef WINDOWEDITOR_H
#define WINDOWEDITOR_H
//-----------------------------------------------------------------------------
#include <QMainWindow>
//-----------------------------------------------------------------------------
namespace Ui {
class WindowEditor;
}
//-----------------------------------------------------------------------------
namespace ImageEditor
{
class ToolsManager;
//-----------------------------------------------------------------------------
class WindowEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowEditor(QWidget *parent = 0);
    ~WindowEditor();

    const QImage *image() const;
    void setImage(const QImage *value);

    QColor color1() const;
    QColor color2() const;
    int scale() const;

    void setTools(ToolsManager *tools);

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    Ui::WindowEditor *ui;
    QImage mImageOriginal;
    QImage mImageScaled;
    QPixmap mPixmapScaled;
    int mScale;
    QColor mColor1;
    QColor mColor2;
    QPixmap mPixmapColor1;
    QPixmap mPixmapColor2;
    bool mFlagChanged;
    ToolsManager *mTools;

    void updateImageScaled(int scale);
    void drawPixel(int x, int y, const QColor &color);

private slots:
    void on_spinBoxScale_valueChanged(int value);
    void on_pushButtonColor1_clicked();
    void on_pushButtonColor2_clicked();

public slots:
    void setScale(int value);
    void setColor1(const QColor value);
    void setColor2(const QColor value);
    void toolChanged(int toolIndex);

signals:
    void imageChanged();
    void mouseMove(const QPoint *point);
    void scaleSchanged(int scale);
};
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
#endif // WINDOWEDITOR_H
