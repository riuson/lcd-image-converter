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
    const QString &codec() const;

    void setBom(bool value);
    void setCodec(const QString &value);

    static const QStringList &codecs();

private:
    bool mBom;
    QString mCodec;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // FONTOPTIONS_H
