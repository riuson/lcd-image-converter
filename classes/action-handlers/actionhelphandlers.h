#ifndef ACTIONHELPHANDLERS_H
#define ACTIONHELPHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
//-----------------------------------------------------------------------------
class ActionHelpHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionHelpHandlers(QObject *parent = 0);
    
signals:
    
public slots:
    void about_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONHELPHANDLERS_H
