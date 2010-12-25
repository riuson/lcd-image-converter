#include "charactersmodel.h"
//-----------------------------------------------------------------------------
CharactersModel::CharactersModel(QObject *parent) :
    QAbstractItemModel(parent)
{
}
//-----------------------------------------------------------------------------
int CharactersModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 65536 / 16 - 2;
}
//-----------------------------------------------------------------------------
int CharactersModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 16;
}
//-----------------------------------------------------------------------------
QVariant CharactersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant result;
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Vertical)
        {
            result = QString("%1").arg((section + 2) * 16, 4, 16);
        }
        else
        {
            result = QString("%1").arg((section), 2, 16);
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QVariant CharactersModel::data(const QModelIndex &index, int role) const
{
    QVariant result = QVariant();
    if (role == Qt::DisplayRole)
    {
        if (index.isValid())
        {
            quint16 a = index.column() + index.row() * 16 + 32;
            result = QString(QChar(a));
        }
    }
    return result;
}
//-----------------------------------------------------------------------------
QModelIndex CharactersModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return this->createIndex(row, column);
}
//-----------------------------------------------------------------------------
QModelIndex CharactersModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
