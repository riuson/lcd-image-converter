#ifndef BYTELISTITEMDELEGATE_H
#define BYTELISTITEMDELEGATE_H
//-----------------------------------------------------------------------------
#include <QtGui>
//-----------------------------------------------------------------------------
class ByteListItemDelegate : public QItemDelegate
{
public:
    ByteListItemDelegate(QObject *pobj);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

public:
    int bitsCount();
    void setBitsCount(const int value);
    QColor colorOdd() const;
    QColor colorEven() const;
    void setColorOdd(const QColor &value);
    void setColorEven(const QColor &value);
private:
    int mBitsCount;
    QColor mColorOdd;//1 3
    QColor mColorEven;//2 4
};
//-----------------------------------------------------------------------------
#endif // BYTELISTITEMDELEGATE_H
