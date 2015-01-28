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

private:
    Ui::WindowEditor *ui;
};
//-----------------------------------------------------------------------------
} // end of namespace
//-----------------------------------------------------------------------------
#endif // WINDOWEDITOR_H
