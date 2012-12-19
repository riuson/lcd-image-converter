#include "historyrecord.h"

#include <QStringList>
#include <QStringListIterator>
//-----------------------------------------------------------------------------
HistoryRecord::HistoryRecord(
        const QMap<QString, QImage *> *_images,
        const QMap<QString, QVariant> *_info,
        QObject *parent) :
    QObject(parent)
{
    QStringList keys = _images->keys();
    QStringListIterator iterator(keys);
    while (iterator.hasNext())
    {
         QString key = iterator.next();

         QImage *newImage = new QImage(*_images->value(key));

         this->mImageMap.insert(key, newImage);
    }

    keys = _info->keys();
    iterator = QStringListIterator(keys);
    while (iterator.hasNext())
    {
         QString key = iterator.next();

         QVariant value = _info->value(key);

         this->mInfoMap.insert(key, value);
    }
}
//-----------------------------------------------------------------------------
HistoryRecord::~HistoryRecord()
{
    qDeleteAll(this->mImageMap);
    this->mImageMap.clear();
    this->mInfoMap.clear();
}
//-----------------------------------------------------------------------------
const QMap<QString, QImage *> *HistoryRecord::images() const
{
    return &this->mImageMap;
}
//-----------------------------------------------------------------------------
const QMap<QString, QVariant> *HistoryRecord::info() const
{
    return &this->mInfoMap;
}
//-----------------------------------------------------------------------------
