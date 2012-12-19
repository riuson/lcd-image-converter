#include "historykeeper.h"

#include <QStringList>
#include <QStringListIterator>
#include "historyrecord.h"
//-----------------------------------------------------------------------------
HistoryKeeper::HistoryKeeper(QObject *parent) :
    QObject(parent)
{
    this->mHistory = new QList<HistoryRecord *>();
    this->mCurrentIndex = -1;
}
//-----------------------------------------------------------------------------
HistoryKeeper::~HistoryKeeper()
{
    qDeleteAll(*this->mHistory);
    this->mHistory->clear();
}
//-----------------------------------------------------------------------------
void HistoryKeeper::store(
        const QMap<QString, QImage *> *images,
        const QMap<QString, QVariant> *info)
{
    this->removeAfter(this->mCurrentIndex);

    HistoryRecord *record = new HistoryRecord(images, info, this);
    this->mHistory->append(record);
    this->mCurrentIndex++;
}
//-----------------------------------------------------------------------------
void HistoryKeeper::restorePrevious(
        QMap<QString, QImage *> *images,
        QMap<QString, QVariant> *info)
{
    if (this->canRestorePrevious())
    {
        this->restoreAt(this->mCurrentIndex, images, info);
        this->mCurrentIndex--;
    }
}
//-----------------------------------------------------------------------------
void HistoryKeeper::restoreNext(
        QMap<QString, QImage *> *images,
        QMap<QString, QVariant> *info)
{
    if (this->canRestoreNext())
    {
        this->mCurrentIndex++;
        this->restoreAt(this->mCurrentIndex, images, info);
    }
}
//-----------------------------------------------------------------------------
bool HistoryKeeper::canRestorePrevious() const
{
    return (this->mCurrentIndex >= 0);
}
//-----------------------------------------------------------------------------
bool HistoryKeeper::canRestoreNext() const
{
    return (this->mCurrentIndex < (this->mHistory->length() - 1));
}
//-----------------------------------------------------------------------------
void HistoryKeeper::removeAfter(int index)
{
    for (int i = this->mHistory->length() - 1; i > index; i--)
    {
        HistoryRecord *record = this->mHistory->at(i);
        this->mHistory->removeAt(i);
        delete record;
    }
}
//-----------------------------------------------------------------------------
void HistoryKeeper::restoreAt(
        int index,
        QMap<QString, QImage *> *images,
        QMap<QString, QVariant> *info)
{
    HistoryRecord *record = this->mHistory->at(index);

    qDeleteAll(*images);
    images->clear();
    info->clear();

    QStringList keys = record->images()->keys();
    QStringListIterator iterator(keys);
    while (iterator.hasNext())
    {
         QString key = iterator.next();

         QImage *oldImage = record->images()->value(key);
         QImage *newImage = new QImage(*oldImage);

         images->insert(key, newImage);
    }

    keys = record->info()->keys();
    iterator = QStringListIterator(keys);
    while (iterator.hasNext())
    {
         QString key = iterator.next();

         QVariant value = record->info()->value(key);

         info->insert(key, value);
    }
}
//-----------------------------------------------------------------------------
