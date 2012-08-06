#ifndef DIALOGPREVIEW_H
#define DIALOGPREVIEW_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
class DialogPreview;
}
class Converter;
class IDataContainer;
//-----------------------------------------------------------------------------
class DialogPreview : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogPreview(IDataContainer *dataContainer, QWidget *parent = 0);
    ~DialogPreview();
    
private:
    Ui::DialogPreview *ui;

    IDataContainer *mData;
    QImage mImageOriginal;
    QImage mImageProcessed;

private slots:
    void on_comboBoxSampleKey_currentIndexChanged();
    void updatePreview();
};
//-----------------------------------------------------------------------------
#endif // DIALOGPREVIEW_H
