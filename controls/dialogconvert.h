#ifndef DIALOGCONVERT_H
#define DIALOGCONVERT_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class DialogConvert;
}
class Converter;
class IDataContainer;
//-----------------------------------------------------------------------------
class DialogConvert : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConvert(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogConvert();

private:
    Ui::DialogConvert *ui;
    QWidget *mWidgetSetup;
    Converter *mConverter;
    IDataContainer *mData;
private slots:
    void on_comboBoxType_currentIndexChanged();
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT_H
