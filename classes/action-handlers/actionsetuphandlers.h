#ifndef ACTIONSETUPHANDLERS_H
#define ACTIONSETUPHANDLERS_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "actionhandlersbase.h"
//-----------------------------------------------------------------------------
class IMainWindow;
//-----------------------------------------------------------------------------
class ActionSetupHandlers : public ActionHandlersBase
{
    Q_OBJECT
public:
    explicit ActionSetupHandlers(QObject *parent = 0);
    
signals:
    
public slots:
    void conversion_triggered();
    void templates_triggered();
};
//-----------------------------------------------------------------------------
#endif // ACTIONSETUPHANDLERS_H
