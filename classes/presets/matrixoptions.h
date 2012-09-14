#ifndef MATRIXOPTIONS_H
#define MATRIXOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
template <class T> class QVector;
//-----------------------------------------------------------------------------
class MatrixOptions : public QObject
{
    Q_OBJECT
public:
    explicit MatrixOptions(QObject *parent = 0);
    virtual ~MatrixOptions();

    quint32 maskUsed() const;
    quint32 maskAnd() const;
    quint32 maskOr() const;
    quint32 maskFill() const;

    void setMaskUsed(quint32 value);
    void setMaskAnd(quint32 value);
    void setMaskOr(quint32 value);
    void setMaskFill(quint32 value);

    int operationsCount() const;
    void operation(int index, quint32 *mask, int *shift, bool *left) const;
    void operationAdd(quint32 mask, int shift, bool left);
    void operationRemove(int index);
    void operationsRemoveAll();
    void operationReplace(int index, quint32 mask, int shift, bool left);

private:
    quint32 mMaskUsed;
    quint32 mMaskAnd;
    quint32 mMaskOr;
    quint32 mMaskFill;
    QVector<quint32> *mOperations;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // MATRIXOPTIONS_H
