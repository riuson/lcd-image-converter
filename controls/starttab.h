#ifndef STARTTAB_H
#define STARTTAB_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class starttab;
}
//-----------------------------------------------------------------------------
class StartTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit StartTab(QWidget *parent = 0);
    ~StartTab();

    void setRecentFiles(const QStringList *list);
    
private:
    Ui::starttab *ui;
};
//-----------------------------------------------------------------------------
#endif // STARTTAB_H
