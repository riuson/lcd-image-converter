#ifndef SETUPTABFONT_H
#define SETUPTABFONT_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabFont;
}
//-----------------------------------------------------------------------------
class Preset;
//-----------------------------------------------------------------------------
class SetupTabFont : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabFont(Preset *preset, QWidget *parent = 0);
    virtual ~SetupTabFont();
    static const QString &title();

public slots:
    void matrixChanged();

private:
    Ui::SetupTabFont *ui;
    Preset *mPreset;

private slots:
    void on_checkBoxBom_toggled(bool value);
    void on_comboBoxCodec_currentIndexChanged(const QString &value);
};
//-----------------------------------------------------------------------------
#endif // SETUPTABFONT_H
