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
class ConverterColor;
class IConverter;
//-----------------------------------------------------------------------------
class WidgetConvOptionsColor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsColor(IConverter *options, QWidget *parent = 0);
    ~WidgetConvOptionsColor();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsColor *ui;
    ByteListItemDelegate *mDelegate;
    QButtonGroup *mGroupByteOrder;
    QButtonGroup *mGroupDataSize;
    ConverterColor *mConv;
    bool mReady;
private slots:
    void updatePreview();
signals:
    void settingsChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSCOLOR_H
