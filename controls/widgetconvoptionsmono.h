#ifndef WIDGETCONVOPTIONSMONO_H
#define WIDGETCONVOPTIONSMONO_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsMono;
}
class ByteListItemDelegate;
class QButtonGroup;
class ConverterMono;
class IConverter;
//-----------------------------------------------------------------------------
class WidgetConvOptionsMono : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsMono(IConverter *options, QWidget *parent = 0);
    ~WidgetConvOptionsMono();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsMono *ui;
    ByteListItemDelegate *mDelegate;
    QButtonGroup *mGroupByteOrder;
    QButtonGroup *mGroupDataSize;
    ConverterMono *mConv;
    bool mReady;
private slots:
    void updatePreview();
signals:
    void settingsChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSMONO_H
