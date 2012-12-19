#ifndef HISTORYKEEPER_H
#define HISTORYKEEPER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QList>
#include <QMap>

//-----------------------------------------------------------------------------
class HistoryRecord;
//-----------------------------------------------------------------------------
class HistoryKeeper : public QObject
{
    Q_OBJECT
public:
    explicit HistoryKeeper(QObject *parent = 0);
    ~HistoryKeeper();

    void storeData(
            const QMap<QString, QImage *> *images,
            const QMap<QString, QVariant> *info);
    void restoreData(
            QMap<QString, QImage *> *images,
            QMap<QString, QVariant> *info);

private:
    QList<HistoryRecord *> *mHistory;

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // HISTORYKEEPER_H
