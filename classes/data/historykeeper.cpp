#include "historykeeper.h"

#include <QStringList>
#include <QStringListIterator>
#include "historyrecord.h"
//-----------------------------------------------------------------------------
HistoryKeeper::HistoryKeeper(QObject *parent) :
    QObject(parent)
{
    this->mHistory = new QList<HistoryRecord *>();
}
//-----------------------------------------------------------------------------
HistoryKeeper::~HistoryKeeper()
{
    qDeleteAll(*this->mHistory);
    this->mHistory->clear();
}
//-----------------------------------------------------------------------------
void HistoryKeeper::storeData(
        const QMap<QString, QImage *> *images,
        const QMap<QString, QVariant> *info)
{
    HistoryRecord *record = new HistoryRecord(images, info, this);
    this->mHistory->append(record);
}
//-----------------------------------------------------------------------------
void HistoryKeeper::restoreData(
        QMap<QString, QImage *> *images,
        QMap<QString, QVariant> *info)
{
    HistoryRecord *record = this->mHistory->last();

    qDeleteAll(*images);
    images->clear();
    info->clear();

    QStringList keys = record->images()->keys();
    QStringListIterator iterator(keys);
    while (iterator.hasNext())
    {
         QString key = iterator.next();

         QImage *newImage = new QImage(*record->images()->value(key));

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
