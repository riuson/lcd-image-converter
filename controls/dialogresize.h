#ifndef DIALOGRESIZE_H
#define DIALOGRESIZE_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogResize;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class DialogResize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogResize(int width, int height, int offsetX, int offsetY, bool center, QWidget *parent = 0);
    ~DialogResize();
    void getResizeInfo(int *width, int *height, int *offsetX, int *offsetY, bool *center);

private:
    Ui::DialogResize *ui;
    int mWidth;
    int mHeight;
    int mOffsetX;
    int mOffsetY;
    bool mCenter;
private slots:
    void on_checkBoxCenter_stateChanged(int state);
    void on_spinBoxWidth_valueChanged(int value);
    void on_spinBoxHeight_valueChanged(int value);
    void on_spinBoxOffsetX_valueChanged(int value);
    void on_spinBoxOffsetY_valueChanged(int value);
};
//-----------------------------------------------------------------------------
#endif // DIALOGRESIZE_H
