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
    void assignImage(QImage *image);

    void inverse();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::WidgetBitmapEditor *ui;
    QImage *mImageOriginal;
    QPixmap mPixmapScaled;
    quint32 mScale;
    QColor mColor1;
    QColor mColor2;
    QPixmap mPixmapColor1;
    QPixmap mPixmapColor2;
    bool mFlagChanged;

    void createImageScaled(quint32 scale);
    void updateState();
private slots:
    void on_spinBoxScale_valueChanged(int value);
    void on_pushButtonColor1_clicked();
    void on_pushButtonColor2_clicked();
signals:
    void dataChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETBITMAPEDITOR_H
