#ifndef ACTIONIMAGEHANDLERS_H
#define ACTIONIMAGEHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

//-----------------------------------------------------------------------------
class IMainWindow;
class WidgetBitmapEditor;

class ActionImageHandlers : public QObject
{
    Q_OBJECT
public:
    explicit ActionImageHandlers(QObject *parent = 0);

private:
    IMainWindow *mMainWindow;
    WidgetBitmapEditor *editor();

signals:
    
public slots:
    void on_actionImageFlip_Horizontal_triggered();
    void on_actionImageFlip_Vertical_triggered();
    void on_actionImageRotate_90_Clockwise_triggered();
    void on_actionImageRotate_180_triggered();
    void on_actionImageRotate_90_Counter_Clockwise_triggered();
    void on_actionImageInverse_triggered();
    void on_actionImageResize_triggered();
    void on_actionImageImport_triggered();
    void on_actionImageExport_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONIMAGEHANDLERS_H
