#ifndef SETUPTABPREPARE_H
#define SETUPTABPREPARE_H

#include <QWidget>

namespace Ui {
class SetupTabPrepare;
}

class SetupTabPrepare : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetupTabPrepare(QWidget *parent = 0);
    ~SetupTabPrepare();
    
private:
    Ui::SetupTabPrepare *ui;
};

#endif // SETUPTABPREPARE_H
