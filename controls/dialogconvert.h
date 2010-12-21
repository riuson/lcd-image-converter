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
    QImage mImageOriginal;
    QImage mImageProcessed;
private slots:
    void on_comboBoxType_currentIndexChanged();
    void on_comboBoxSampleKey_currentIndexChanged();
    void updatePreview();
};
//-----------------------------------------------------------------------------
#endif // DIALOGCONVERT_H
