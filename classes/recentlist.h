#ifndef RECENTLIST_H
#define RECENTLIST_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
//class QStringList;
//class QStringListIterator;
//-----------------------------------------------------------------------------
class RecentList : public QObject
{
    Q_OBJECT
public:
    explicit RecentList(QObject *parent = 0);
    virtual ~RecentList();
    void add(const QString &filename);
    enum { MaxRecentFiles = 10 };
    const QStringList * files() const;

private:
    QStringList *mFiles;
signals:
    void listChanged();
public slots:
    
};
//-----------------------------------------------------------------------------
#endif // RECENTLIST_H
