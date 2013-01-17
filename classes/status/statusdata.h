#ifndef STATUSDATA_H
#define STATUSDATA_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QMap>
#include <QStringList>
//-----------------------------------------------------------------------------
class StatusData : public QObject
{
    Q_OBJECT
public:
    explicit StatusData(QObject *parent = 0);

    const QStringList keys() const;
    const QString data(const QString &key) const;
    void setData(const QString &key, const QString &value);

private:
    QMap<QString, QString> mData;
};
//-----------------------------------------------------------------------------
#endif // STATUSDATA_H
