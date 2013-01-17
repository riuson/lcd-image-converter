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
    if (value.isNull())
    {
        this->mData.remove(key);
    }
    else
    {
        this->mData.insert(key, value);
    }
}
//-----------------------------------------------------------------------------
