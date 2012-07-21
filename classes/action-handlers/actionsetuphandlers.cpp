#include "actionsetuphandlers.h"

#include "dialogsetuptemplates.h"
#include "dialogconvert.h"
#include "widgetbitmapeditor.h"
#include "imainwindow.h"
#include "idatacontainer.h"
//-----------------------------------------------------------------------------
ActionSetupHandlers::ActionSetupHandlers(QObject *parent) :
    ActionHandlersBase(parent)
{
}
//-----------------------------------------------------------------------------
void ActionSetupHandlers::on_actionSetupConversion_triggered()
{
    IDataContainer *data = NULL;
    if (this->editor() != NULL)
        data = this->editor()->dataContainer();
    DialogConvert dialog(data, this->mMainWindow->parentWidget());
    dialog.exec();
}
//-----------------------------------------------------------------------------
void ActionSetupHandlers::on_actionSetupTemplates_triggered()
{
    DialogSetupTemplates dialog(this->mMainWindow->parentWidget());
    dialog.exec();
}
//-----------------------------------------------------------------------------
