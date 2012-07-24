#ifndef ACTIONIMAGEHANDLERS_H
#define ACTIONIMAGEHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
class WidgetBitmapEditor;
//-----------------------------------------------------------------------------
class ActionImageHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionImageHandlers(QObject *parent = 0);

signals:
    
public slots:
    void flipHorizontal_triggered();
    void flipVertical_triggered();
    void rotate_90_Clockwise_triggered();
    void rotate_180_triggered();
    void rotate_90_Counter_Clockwise_triggered();
    void inverse_triggered();
    void resize_triggered();
    void import_triggered();
    void export_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONIMAGEHANDLERS_H
