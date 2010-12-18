#ifndef DIALOGSAVECHANGES_H
#define DIALOGSAVECHANGES_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogSaveChanges;
}
//-----------------------------------------------------------------------------
class DialogSaveChanges : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSaveChanges(QWidget *parent = 0);
    ~DialogSaveChanges();
    enum SaveChangesResultCode
    {
        Save = 1,
        SaveAs = 2,
        DontSave = 3,
        Cancel = 0
    };
    int answer();

private:
    Ui::DialogSaveChanges *ui;
    SaveChangesResultCode mAnswer;
private slots:
    void on_buttonSave_clicked();
    void on_buttonSaveAs_clicked();
    void on_buttonDontSave_clicked();
    void on_buttonCancel_clicked();
};
//-----------------------------------------------------------------------------
#endif // DIALOGSAVECHANGES_H
