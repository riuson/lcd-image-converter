#include "actionhandlersbase.h"

#include "idocument.h"
#include "imainwindow.h"
//-----------------------------------------------------------------------------
ActionHandlersBase::ActionHandlersBase(QObject *parent) :
    QObject(parent)
{
    this->mMainWindow = qobject_cast<IMainWindow *>(parent);
}
//-----------------------------------------------------------------------------
WidgetBitmapEditor *ActionHandlersBase::editor()
{
    IDocument *doc = this->mMainWindow->currentDocument();
    if (doc != NULL)
    {
        return doc->editor();
    }
    return NULL;
}
//-----------------------------------------------------------------------------
