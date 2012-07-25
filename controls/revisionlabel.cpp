#include "revisionlabel.h"
//-----------------------------------------------------------------------------
#include <QFile>
#include <QString>
#include <QTextStream>
//-----------------------------------------------------------------------------
RevisionLabel::RevisionLabel(QWidget *parent) :
    QLabel(parent)
{
    // load version info
    QFile file_version(":/text/version_info");
    if (file_version.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file_version);
        QString version = stream.readAll();
        file_version.close();

        // git-commit-info d693078 Sat May 26 22:30:51 2012 +0600

        QString start = "git-commit-info ";
        if (version.startsWith(start, Qt::CaseInsensitive))
        {
            // get hash of commit
            QRegExp reg = QRegExp("[0-9a-f]+", Qt::CaseInsensitive);
            int index;
            if ((index = reg.indexIn(version, start.length())) >= 0)
            {
                QString hash = reg.cap();

                this->setText(hash);
            }
        }
    }
    else
    {
        this->setText("");
    }

    this->setForegroundRole(QPalette::Midlight);
}
//-----------------------------------------------------------------------------
