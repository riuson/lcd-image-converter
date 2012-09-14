#ifndef SETUPTABMATRIX_H
#define SETUPTABMATRIX_H

#include <QWidget>

namespace Ui {
class SetupTabMatrix;
}

class SetupTabMatrix : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetupTabMatrix(QWidget *parent = 0);
    ~SetupTabMatrix();
    
private:
    Ui::SetupTabMatrix *ui;
};

#endif // SETUPTABMATRIX_H
