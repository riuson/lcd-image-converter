#ifndef MATRIXPREVIEWMODEL_H
#define MATRIXPREVIEWMODEL_H
//-----------------------------------------------------------------------------
#include <QAbstractItemModel>
#include <QVariant>

#include "conversionmatrixoptions.h"
//-----------------------------------------------------------------------------
class MatrixPreviewModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MatrixPreviewModel(QList<quint32> *matrix, QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    QList<quint32> *mMatrix;

    enum RowType
    {
        Source,
        Operation,
        MaskUsed,
        MaskAnd,
        MaskOr,
        Result
    };
    enum ColorType
    {
        Empty,
        // for colors
        Red,
        Green,
        Blue,
        // for monochrome
        BlackOrWhite,
        //for grayscale
        Gray
    };

    void getBitType(int bitIndex, ConversionType *convType, ColorType *colorType, int *partIndex) const;
    RowType rowType(int row) const;
    void resultToSourceBit(int bitIndex, QVariant *name, QVariant *color) const;
    void sourceBitProperties(int bitIndex, QVariant *name, QVariant *color) const;

signals:
    
public slots:
    
};
//-----------------------------------------------------------------------------
#endif // MATRIXPREVIEWMODEL_H
