#ifndef FONTHELPER_H
#define FONTHELPER_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include <QImage>
//-----------------------------------------------------------------------------
class IDataContainer;
//-----------------------------------------------------------------------------
class FontHelper : public QObject
{
    Q_OBJECT
public:
    explicit FontHelper(QObject *parent = 0);

    static QImage drawString(const IDataContainer *data, const QString &value);
signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTHELPER_H
