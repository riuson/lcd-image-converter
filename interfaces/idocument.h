#ifndef IDOCUMENT_H
#define IDOCUMENT_H
//-----------------------------------------------------------------------------
#include <QObject>

class QString;
class IDataContainer;
//-----------------------------------------------------------------------------
class IDocument
{
public:
    virtual bool save(const QString &filename) = 0;
    virtual bool load(const QString &filename) = 0;
    virtual bool changed() = 0;
    virtual QString fileName() = 0;
    virtual QString documentName() = 0;
    virtual void setDocumentName(const QString &value) = 0;
    virtual IDataContainer *dataContainer() = 0;
};
Q_DECLARE_INTERFACE (IDocument,
                     "riuson.lcd-image-converter/1.0"
                     )
//-----------------------------------------------------------------------------
#endif // IDOCUMENT_H
