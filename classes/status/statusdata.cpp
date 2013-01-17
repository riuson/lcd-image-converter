#include "statusdata.h"
//-----------------------------------------------------------------------------
StatusData::StatusData(QObject *parent) :
    QObject(parent)
{
}
//-----------------------------------------------------------------------------
const QStringList StatusData::keys() const
{
    return this->mData.keys();
}
//-----------------------------------------------------------------------------
const QString StatusData::data(const QString &key) const
{
    return this->mData.value(key, QString());
}
//-----------------------------------------------------------------------------
void StatusData::setData(const QString &key, const QString &value)
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
