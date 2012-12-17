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

#ifndef WIDGETBITMAPEDITOR_H
#define WIDGETBITMAPEDITOR_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetBitmapEditor;
}
//-----------------------------------------------------------------------------
class WidgetBitmapEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBitmapEditor(QWidget *parent = 0);
    ~WidgetBitmapEditor();

    const QImage *currentImage() const;
    void setCurrentImage(const QImage *value);

    QColor color1();
    QColor color2();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::WidgetBitmapEditor *ui;
    QImage mImageOriginal;
    QImage mImageScaled;
    QPixmap mPixmapScaled;
    int mScale;
    QColor mColor1;
    QColor mColor2;
    QPixmap mPixmapColor1;
    QPixmap mPixmapColor2;
    bool mFlagChanged;

    void updateImageScaled(int scale);
    void updateState();
    void drawPixel(int x, int y, const QColor &color);
private slots:
    void on_spinBoxScale_valueChanged(int value);
    void on_pushButtonColor1_clicked();
    void on_pushButtonColor2_clicked();
    void mon_dataContainer_imageChanged(const QString &key);
signals:
    void imageChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETBITMAPEDITOR_H
