#ifndef EDITORTABIMAGE_H
#define EDITORTABIMAGE_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabImage;
}
//-----------------------------------------------------------------------------
class WidgetBitmapEditor;
//-----------------------------------------------------------------------------
class EditorTabImage : public QWidget
{
    Q_OBJECT

public:
    explicit EditorTabImage(QWidget *parent = 0);
    ~EditorTabImage();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabImage *ui;
    WidgetBitmapEditor *mEditor;
};
//-----------------------------------------------------------------------------
#endif // EDITORTABIMAGE_H
