#ifndef FONTCONTAINER_H
#define FONTCONTAINER_H

#include <QObject>

#include <QMap>
#include <QString>

#include "idatacontainer.h"
//-----------------------------------------------------------------------------
class QImage;
//-----------------------------------------------------------------------------
class FontContainer : public QObject, public IDataContainer
{
    Q_OBJECT
    Q_INTERFACES(IDataContainer)
public:
    explicit FontContainer(QObject *parent = 0);
    ~FontContainer();
private:
    QMap<QString, QImage *> mImageMap;
    QImage *mDefaultImage;
public:
    QImage *image(const QString &key);
    void setImage(const QString &key, QImage *image);
    void transform(const QString &key, int code);
    int count();

    void clear();
    QList<QString> keys();
    void remove(const QString &key);
signals:
    void imageChanged(const QString &key);
public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTCONTAINER_H
