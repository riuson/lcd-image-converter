#include "recentlist.h"
//-----------------------------------------------------------------------------
#include <QStringList>
#include <QStringListIterator>
#include <QSettings>
//-----------------------------------------------------------------------------
RecentList::RecentList(QObject *parent) :
    QObject(parent)
{
    this->mFiles = new QStringList();

    // load from settings
    QSettings sett;
    int size = sett.beginReadArray("recent");
    for (int i = 0; i < size; i++)
    {
        sett.setArrayIndex(i);
        this->mFiles->append(sett.value("filename").toString());
    }
    sett.endArray();
}
//-----------------------------------------------------------------------------
RecentList::~RecentList()
{
    // save to settings
    QSettings sett;
    sett.beginWriteArray("recent");
    QStringListIterator recentFilesIterator(*this->mFiles);
    int i = 0;
    while (recentFilesIterator.hasNext())
    {
        sett.setArrayIndex(i++);
        sett.setValue("filename", recentFilesIterator.next());
    }
    sett.endArray();

    delete this->mFiles;
}
//-----------------------------------------------------------------------------
void RecentList::add(const QString &filename)
{
    if (this->mFiles->contains(filename))
        this->mFiles->removeOne(filename);
    this->mFiles->insert(0, filename);

    emit this->listChanged();
}
//-----------------------------------------------------------------------------
const QStringList * RecentList::files() const
{
    return this->mFiles;
}
//-----------------------------------------------------------------------------
