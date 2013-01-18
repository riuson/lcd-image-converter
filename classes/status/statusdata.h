#ifndef STATUSDATA_H
#define STATUSDATA_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QVariant>
#include <QMap>
#include <QList>
//-----------------------------------------------------------------------------
class StatusData : public QObject
{
    Q_OBJECT
public:

    enum StatusType
    {
        ImageIndex,
        MouseCoordinates,
        Scale
    };

    explicit StatusData(QObject *parent = 0);

    const QList<StatusType> keys() const;
    const QVariant data(const StatusType key) const;
    void setData(const StatusType key, const QVariant &value);
    void removeData(const StatusType key);

private:
    QMap<StatusType, QVariant> mData;

signals:
    void changed();
};
//-----------------------------------------------------------------------------
#endif // STATUSDATA_H
