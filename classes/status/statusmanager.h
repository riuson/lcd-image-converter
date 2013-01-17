#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QMap>
#include "statusdata.h"
//-----------------------------------------------------------------------------
class QStatusBar;
class QLabel;
//-----------------------------------------------------------------------------
class StatusManager : public QObject
{
    Q_OBJECT
public:
    explicit StatusManager(QStatusBar *statusBar, QObject *parent = 0);
    ~StatusManager();

    void updateData(const StatusData *statuses);

private:
    QStatusBar *mBar;
    QMap<StatusData::StatusType, QLabel *> mList;

    void updateItem(StatusData::StatusType type, QLabel *label, const StatusData *statuses);

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // STATUSMANAGER_H
