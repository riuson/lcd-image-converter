#ifndef SETUPTABIMAGE_H
#define SETUPTABIMAGE_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabImage;
}
//-----------------------------------------------------------------------------
class Preset;
//-----------------------------------------------------------------------------
class SetupTabImage : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabImage(Preset *preset, QWidget *parent = 0);
    virtual ~SetupTabImage();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabImage *ui;
    Preset *mPreset;

private slots:
    void on_checkBoxSplitToRows_toggled(bool value);
    void on_radioButtonLittleEndian_toggled(bool value);
    void on_comboBoxBlockSize_currentIndexChanged(int index);
};
//-----------------------------------------------------------------------------
#endif // SETUPTABIMAGE_H