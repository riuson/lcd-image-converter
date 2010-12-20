#ifndef WIDGETCONVOPTIONSGRAY_H
#define WIDGETCONVOPTIONSGRAY_H
//-----------------------------------------------------------------------------
#include <QDialog>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsGray;
}
class ByteListItemDelegate;
class QButtonGroup;
//-----------------------------------------------------------------------------
class WidgetConvOptionsGray : public QDialog
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsGray(QWidget *parent = 0);
    ~WidgetConvOptionsGray();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsGray *ui;
    ByteListItemDelegate *mDelegate;
    QButtonGroup *mGroupByteOrder;
    QButtonGroup *mGroupDataSize;
private slots:
    void updatePreview();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSGRAY_H
