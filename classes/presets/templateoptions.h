#ifndef TEMPLATEOPTIONS_H
#define TEMPLATEOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QString>
//-----------------------------------------------------------------------------
class TemplateOptions : public QObject
{
    Q_OBJECT
public:
    explicit TemplateOptions(QObject *parent = 0);

    const QString &image() const;
    const QString &font() const;

    void setImage(const QString &filename);
    void setFont(const QString &filename);

private:
    QString mImageTemplate;
    QString mFontTemplate;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // TEMPLATEOPTIONS_H
