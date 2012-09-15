#ifndef IMAGEOPTIONS_H
#define IMAGEOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class ImageOptions : public QObject
{
    Q_OBJECT
public:
    explicit ImageOptions(QObject *parent = 0);

    BytesOrder bytesOrder() const;
    DataBlockSize blockSize() const;

    void setBytesOrder(BytesOrder value);
    void setBlockSize(DataBlockSize value);

private:
    BytesOrder mBytesOrder;
    DataBlockSize mBlockSize;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // IMAGEOPTIONS_H
