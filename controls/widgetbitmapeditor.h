#ifndef WIDGETBITMAPEDITOR_H
#define WIDGETBITMAPEDITOR_H
//-----------------------------------------------------------------------------
#include <QWidget>
//-----------------------------------------------------------------------------
namespace Ui {
    class WidgetBitmapEditor;
}
class IDataContainer;
//-----------------------------------------------------------------------------
class WidgetBitmapEditor : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBitmapEditor(IDataContainer *dataContainer, QWidget *parent = 0);
    ~WidgetBitmapEditor();

    IDataContainer *dataContainer();

    void selectImage(int index);
    int currentImageIndex();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::WidgetBitmapEditor *ui;
    QPixmap mPixmapScaled;
    int mScale;
    QColor mColor1;
    QColor mColor2;
    QPixmap mPixmapColor1;
    QPixmap mPixmapColor2;
    bool mFlagChanged;
    IDataContainer *mDataContainer;
    int mImageIndex;

    void createImageScaled(int scale);
    void updateState();
private slots:
    void on_spinBoxScale_valueChanged(int value);
    void on_pushButtonColor1_clicked();
    void on_pushButtonColor2_clicked();
    void on_dataContainer_imageChanged(int index);
signals:
    void dataChanged();
};
//-----------------------------------------------------------------------------
#endif // WIDGETBITMAPEDITOR_H
