#ifndef EDITORTABFONT_H
#define EDITORTABFONT_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class EditorTabFont;
}
//-----------------------------------------------------------------------------
class FontContainer;
class WidgetBitmapEditor;
//-----------------------------------------------------------------------------
class EditorTabFont : public QWidget
{
    Q_OBJECT

public:
    explicit EditorTabFont(QWidget *parent = 0);
    ~EditorTabFont();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::EditorTabFont *ui;
    WidgetBitmapEditor *mEditor;
    FontContainer *mContainer;
};
//-----------------------------------------------------------------------------
#endif // EDITORTABFONT_H
