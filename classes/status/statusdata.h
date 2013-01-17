#ifndef STATUSDATA_H
#define STATUSDATA_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QMap>
#include <QList>
//-----------------------------------------------------------------------------
class StatusData : public QObject
{
    Q_OBJECT
public:

    enum StatusType
    {
        ImageIndex
    };

    explicit StatusData(QObject *parent = 0);

    const QList<StatusType> keys() const;
    const QString data(const StatusType key) const;
    void setData(const StatusType key, const QString &value);

private:
    QMap<StatusType, QString> mData;
};
//-----------------------------------------------------------------------------
#endif // STATUSDATA_H
