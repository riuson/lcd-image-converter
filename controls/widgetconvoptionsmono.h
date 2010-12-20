#ifndef WIDGETCONVOPTIONSMONO_H
#define WIDGETCONVOPTIONSMONO_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsMono;
}
class ByteListItemDelegate;
//-----------------------------------------------------------------------------
class WidgetConvOptionsMono : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsMono(QWidget *parent = 0);
    ~WidgetConvOptionsMono();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsMono *ui;
    ByteListItemDelegate *mDelegate;
private slots:
    void updatePreview();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSMONO_H
