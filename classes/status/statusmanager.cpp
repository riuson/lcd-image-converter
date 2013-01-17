#include "statusmanager.h"

#include <QStatusBar>
#include "revisionlabel.h"
//-----------------------------------------------------------------------------
StatusManager::StatusManager(QStatusBar *statusBar, QObject *parent) :
    QObject(parent)
{
    this->mBar = statusBar;

    this->mBar->addWidget(new RevisionLabel);
}
//-----------------------------------------------------------------------------
void StatusManager::updateData(const StatusData *data)
{
}
//-----------------------------------------------------------------------------
