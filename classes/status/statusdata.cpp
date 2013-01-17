#include "statusdata.h"
//-----------------------------------------------------------------------------
StatusData::StatusData(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
const QList<StatusData::StatusType> StatusData::keys() const
{
    return this->mData.keys();
}
//-----------------------------------------------------------------------------
const QVariant StatusData::data(const StatusType key) const
{
    return this->mData.value(key, QString());
}
//-----------------------------------------------------------------------------
void StatusData::setData(const StatusType key, const QVariant &value)
{
    this->mData.insert(key, value);
}
//-----------------------------------------------------------------------------
void StatusData::removeData(const StatusData::StatusType key)
{
    this->mData.remove(key);
}
//-----------------------------------------------------------------------------
