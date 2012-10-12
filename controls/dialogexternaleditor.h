#ifndef DIALOGEXTERNALEDITOR_H
#define DIALOGEXTERNALEDITOR_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogExternalEditor;
}
//-----------------------------------------------------------------------------
class DialogExternalEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExternalEditor(QWidget *parent = 0);
    ~DialogExternalEditor();

private:
    Ui::DialogExternalEditor *ui;

private slots:
    void on_pushButtonBrowse_clicked();

protected:
    void done(int result);
};
//-----------------------------------------------------------------------------
#endif // DIALOGEXTERNALEDITOR_H
