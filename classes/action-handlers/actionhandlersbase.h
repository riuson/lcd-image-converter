#ifndef ACTIONHANDLERSBASE_H
#define ACTIONHANDLERSBASE_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class IMainWindow;
class WidgetBitmapEditor;
//-----------------------------------------------------------------------------
class ActionHandlersBase : public QObject
{
    Q_OBJECT
public:
    explicit ActionHandlersBase(QObject *parent = 0);
    
protected:
    IMainWindow *mMainWindow;
    WidgetBitmapEditor *editor();

signals:
    
public slots:
    
};
//-----------------------------------------------------------------------------
#endif // ACTIONHANDLERSBASE_H
