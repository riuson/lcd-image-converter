#ifndef ACTIONFONTHANDLERS_H
#define ACTIONFONTHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
class WidgetBitmapEditor;
//-----------------------------------------------------------------------------
class ActionFontHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionFontHandlers(QObject *parent = 0);
    
signals:
    
public slots:
    void fontChange_triggered();
    void fontInverse_triggered();
    void fontResize_triggered();
    void fontMinimizeHeight_triggered();
    void fontPreview_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONFONTHANDLERS_H
