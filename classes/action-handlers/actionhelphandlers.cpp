#include "actionhelphandlers.h"

#include "dialogabout.h"
#include "imainwindow.h"
//-----------------------------------------------------------------------------
ActionHelpHandlers::ActionHelpHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionHelpHandlers::on_actionAbout_triggered()
{
    DialogAbout dialog(this->mMainWindow->parentWidget());
    dialog.exec();
}
//-----------------------------------------------------------------------------
