#ifndef WIDGETCONVOPTIONSGRAY_H
#define WIDGETCONVOPTIONSGRAY_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetConvOptionsGray;
}
class ByteListItemDelegate;
class QButtonGroup;
class ConverterGrayscale;
class IConverter;
//-----------------------------------------------------------------------------
class WidgetConvOptionsGray : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetConvOptionsGray(IConverter *options, QWidget *parent = 0);
    ~WidgetConvOptionsGray();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::WidgetConvOptionsGray *ui;
    ByteListItemDelegate *mDelegate;
    ConverterGrayscale *mConv;
    bool mReady;
private slots:
    void updatePreview();
public slots:
    void dataLengthChanged(int value);
    void dataPackChanged(bool value);
    void swapBytesChanged(bool value);
    void mirrorBytesChanged(bool value);
signals:
    void settingsChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETCONVOPTIONSGRAY_H
