#ifndef FONTOPTIONS_H
#define FONTOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
class QStringList;
//-----------------------------------------------------------------------------
class FontOptions : public QObject
{
    Q_OBJECT
public:
    explicit FontOptions(QObject *parent = 0);

    bool bom() const;
    const QString &encoding() const;

    void setBom(bool value);
    void setEncoding(const QString &value);

    static const QStringList &encodings();

private:
    bool mBom;
    QString mEncoding;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTOPTIONS_H
