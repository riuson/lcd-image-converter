#ifndef SETUPTABIMAGE_H
#define SETUPTABIMAGE_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabImage;
}
//-----------------------------------------------------------------------------
class ConversionMatrix;
//-----------------------------------------------------------------------------
class SetupTabImage : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabImage(ConversionMatrix *matrix, QWidget *parent = 0);
    ~SetupTabImage();
    static const QString &title();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabImage *ui;
    ConversionMatrix *mMatrix;

private slots:
    void on_radioButtonLittleEndian_toggled(bool value);
    void on_comboBoxBlockSize_currentIndexChanged(int index);
};
//-----------------------------------------------------------------------------
#endif // SETUPTABIMAGE_H
