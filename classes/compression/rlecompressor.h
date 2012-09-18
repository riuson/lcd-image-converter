#ifndef RLE_H
#define RLE_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
// Run-length encoding algorithm
// http://en.wikipedia.org/wiki/Run-length_encoding
//-----------------------------------------------------------------------------
template <class T> class QVector;
template <class T> class QQueue;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class RleCompressor : public QObject
{
    Q_OBJECT
public:
    explicit RleCompressor(QObject *parent = 0);

    void compress(QVector<quint32> *input, DataBlockSize dataSize, QVector<quint32> *output);

private:
    void flush(QVector<quint32> *output, QQueue<quint32> *queue);
    bool allEquals(QQueue<quint32> *queue);

signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // RLE_H
