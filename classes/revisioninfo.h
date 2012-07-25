#ifndef REVISIONINFO_H
#define REVISIONINFO_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class RevisionInfo : public QObject
{
    Q_OBJECT
public:
    explicit RevisionInfo(QObject *parent = 0);
    static bool getRevisionData(QString *hash, QString *date);
private:
signals:
    
public slots:
    
};
//-----------------------------------------------------------------------------
#endif // REVISIONINFO_H
