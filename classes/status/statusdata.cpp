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
const QString StatusData::data(const StatusType key) const
{
    return this->mData.value(key, QString());
}
//-----------------------------------------------------------------------------
void StatusData::setData(const StatusType key, const QString &value)
{
    if (value.isEmpty())
    {
        this->mData.remove(key);
    }
    else
    {
        this->mData.insert(key, value);
    }
}
//-----------------------------------------------------------------------------
