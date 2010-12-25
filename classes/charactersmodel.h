#ifndef CHARACTERSMODEL_H
#define CHARACTERSMODEL_H
//-----------------------------------------------------------------------------
#include <QAbstractItemModel>
#include <QVariant>
//-----------------------------------------------------------------------------
class CharactersModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CharactersModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
signals:

public slots:

};
//-----------------------------------------------------------------------------
#endif // CHARACTERSMODEL_H
