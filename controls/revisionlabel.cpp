#include "revisionlabel.h"
//-----------------------------------------------------------------------------
#include "revisioninfo.h"
//-----------------------------------------------------------------------------
RevisionLabel::RevisionLabel(QWidget *parent) :
    QLabel(parent)
{
    QString hash, date;
    // load version info
    if (RevisionInfo::getRevisionData(&hash, &date))
    {
        this->setText(hash);
    }
    else
    {
        this->setText("");
    }

    this->setForegroundRole(QPalette::Midlight);
}
//-----------------------------------------------------------------------------
