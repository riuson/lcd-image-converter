#ifndef WIDGETCONVOPTIONSCOLOR_H
#define WIDGETCONVOPTIONSCOLOR_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsColor;
}
class ByteListItemDelegate;
class QButtonGroup;
//-----------------------------------------------------------------------------
class WidgetConvOptionsColor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsColor(QWidget *parent = 0);
    ~WidgetConvOptionsColor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsColor *ui;
    ByteListItemDelegate *mDelegate;
    QButtonGroup *mGroupByteOrder;
    QButtonGroup *mGroupDataSize;
private slots:
    void updatePreview();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSCOLOR_H
