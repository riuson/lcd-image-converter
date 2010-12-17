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

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetBitmapEditor *ui;
    QImage *mImageOriginal;
    QPixmap mPixmapScaled;
    quint32 mScale;

    void createImageScaled(quint32 scale);
};
//-----------------------------------------------------------------------------
#endif // WIDGETBITMAPEDITOR_H
