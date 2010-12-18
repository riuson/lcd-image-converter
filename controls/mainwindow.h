#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//-----------------------------------------------------------------------------
#include <QMainWindow>
//-----------------------------------------------------------------------------
namespace Ui {
    class MainWindow;
}
//-----------------------------------------------------------------------------
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    QString findAvailableName(const QString &prefix);

private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);
    void on_actionNew_Image_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
};
//-----------------------------------------------------------------------------
#endif // MAINWINDOW_H
