#ifndef SETUPTABTEMPLATES_H
#define SETUPTABTEMPLATES_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
class SetupTabTemplates;
}
//-----------------------------------------------------------------------------
class Preset;
//-----------------------------------------------------------------------------
class SetupTabTemplates : public QWidget
{
    Q_OBJECT

public:
    explicit SetupTabTemplates(Preset *preset, QWidget *parent = 0);
    virtual ~SetupTabTemplates();
    static const QString &title();

private:
    Ui::SetupTabTemplates *ui;
    Preset *mPreset;

private slots:
    void matrixChanged();
    void on_pushButtonBrowseImage_clicked();
    void on_pushButtonBrowseFont_clicked();
};
//-----------------------------------------------------------------------------
#endif // SETUPTABTEMPLATES_H
