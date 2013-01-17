#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QStatusBar;
class StatusData;
//-----------------------------------------------------------------------------
class StatusManager : public QObject
{
    Q_OBJECT
public:
    explicit StatusManager(QStatusBar *statusBar, QObject *parent = 0);

    void updateData(const StatusData *data);

private:
    QStatusBar *mBar;

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // STATUSMANAGER_H
