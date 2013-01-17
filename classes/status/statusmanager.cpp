#include "statusmanager.h"

#include <QStatusBar>
#include "revisionlabel.h"
//-----------------------------------------------------------------------------
StatusManager::StatusManager(QStatusBar *statusBar, QObject *parent) :
    QObject(parent)
{
    this->mBar = statusBar;

    this->mBar->addPermanentWidget(new RevisionLabel(this->mBar));
}
//-----------------------------------------------------------------------------
void StatusManager::updateData(const StatusData *data)
{
}
//-----------------------------------------------------------------------------
