#ifndef SETUPTABPREPARE_H
#define SETUPTABPREPARE_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabPrepare;
}
//-----------------------------------------------------------------------------
class ConversionMatrix;
//-----------------------------------------------------------------------------
class SetupTabPrepare : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabPrepare(ConversionMatrix *matrix, QWidget *parent = 0);
    ~SetupTabPrepare();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabPrepare *ui;
    ConversionMatrix *mMatrix;

private slots:
    void on_comboBoxConversionType_currentIndexChanged(int index);
    void on_comboBoxMonochromeType_currentIndexChanged(int index);
    void on_comboBoxRotate_currentIndexChanged(int index);
    void on_checkBoxFlipHorizontal_toggled(bool value);
    void on_checkBoxFlipVertical_toggled(bool value);
    void on_checkBoxInverse_toggled(bool value);
    void on_horizontalScrollBarEdge_valueChanged(int value);
};
//-----------------------------------------------------------------------------
#endif // SETUPTABPREPARE_H
