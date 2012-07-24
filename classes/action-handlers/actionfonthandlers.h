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
    void on_actionFontChange_triggered();
    void on_actionFontInverse_triggered();
    void on_actionFontResize_triggered();
    void on_actionFontMinimizeHeight_triggered();
    void on_actionFontPreview_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONFONTHANDLERS_H
