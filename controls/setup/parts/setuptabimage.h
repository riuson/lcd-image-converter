#ifndef SETUPTABIMAGE_H
#define SETUPTABIMAGE_H

#include <QWidget>

namespace Ui {
class SetupTabImage;
}

class SetupTabImage : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetupTabImage(QWidget *parent = 0);
    ~SetupTabImage();
    
private:
    Ui::SetupTabImage *ui;
};

#endif // SETUPTABIMAGE_H
