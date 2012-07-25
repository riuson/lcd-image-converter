#include "revisioninfo.h"
//-----------------------------------------------------------------------------
#include <QFile>
#include <QTextStream>
#include <QRegExp>
//-----------------------------------------------------------------------------
// git-commit-info d693078 Sat May 26 22:30:51 2012 +0600
//-----------------------------------------------------------------------------
RevisionInfo::RevisionInfo(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
bool RevisionInfo::getRevisionData(QString *hash, QString *date)
{
    bool result = false;
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
                *hash = reg.cap();

                // get date
                *date = version.mid(index + (*hash).length() + 1);

                result = true;
            }
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
