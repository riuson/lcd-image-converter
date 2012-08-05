#ifndef DIALOGCONVERT2_H
#define DIALOGCONVERT2_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogConvert2;
}
class Converter;
class IDataContainer;
//-----------------------------------------------------------------------------
class DialogConvert2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogConvert2(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogConvert2();
    
private:
    Ui::DialogConvert2 *ui;

    IDataContainer *mData;
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT2_H
