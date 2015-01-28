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

signals:
    void imageChanged();
    void mouseMove(QPoint point);
    void scaleSchanged(int scale);
};
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
#endif // WINDOWEDITOR_H
