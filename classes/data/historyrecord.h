#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QMap>
//-----------------------------------------------------------------------------
class HistoryRecord : public QObject
{
    Q_OBJECT
public:
    explicit HistoryRecord(
            const QMap<QString, QImage *> *_images,
            const QMap<QString, QVariant> *_info,
            QObject *parent = 0);
    ~HistoryRecord();

    const QMap<QString, QImage *> *images() const;
    const QMap<QString, QVariant> *info() const;

private:
    QMap<QString, QImage *> mImageMap;
    QMap<QString, QVariant> mInfoMap;

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // HISTORYRECORD_H
