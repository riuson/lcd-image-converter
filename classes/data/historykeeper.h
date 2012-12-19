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

    void store(
            const QMap<QString, QImage *> *images,
            const QMap<QString, QVariant> *info);
    void restorePrevious(
            QMap<QString, QImage *> *images,
            QMap<QString, QVariant> *info);
    void restoreNext(
            QMap<QString, QImage *> *images,
            QMap<QString, QVariant> *info);

    bool canRestorePrevious() const;
    bool canRestoreNext() const;

private:
    QList<HistoryRecord *> *mHistory;
    int mCurrentIndex;

    void removeAfter(int index);
    void restoreAt(
            int index,
            QMap<QString, QImage *> *images,
            QMap<QString, QVariant> *info);
};
//-----------------------------------------------------------------------------
#endif // HISTORYKEEPER_H
