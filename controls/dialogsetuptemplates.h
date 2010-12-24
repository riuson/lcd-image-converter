#ifndef DIALOGSETUPTEMPLATES_H
#define DIALOGSETUPTEMPLATES_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogSetupTemplates;
}
//-----------------------------------------------------------------------------
class DialogSetupTemplates : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetupTemplates(QWidget *parent = 0);
    ~DialogSetupTemplates();

private:
    Ui::DialogSetupTemplates *ui;
private slots:
    void on_pushButtonBrowseImage_clicked();
    void on_pushButtonBrowseFont_clicked();
};
//-----------------------------------------------------------------------------
#endif // DIALOGSETUPTEMPLATES_H
