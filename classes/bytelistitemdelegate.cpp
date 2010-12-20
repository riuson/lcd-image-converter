#include "bytelistitemdelegate.h"
//-----------------------------------------------------------------------------
ByteListItemDelegate::ByteListItemDelegate(QObject *pobj = 0) : QItemDelegate(pobj)
{
    this->mBitsCount = 8;
    this->mColorEven = QColor("silver");
    this->mColorOdd = QColor("black");
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::paint(QPainter *painter,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    if (index.isValid())
    {
        QRect rect = option.rect;
        bool even = (index.row() / this->mBitsCount) % 2;
        if (even)
        {
            //painter->setPen(this->mColorEven);
            QPen pen(QBrush(this->mColorEven), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.top() + 2, rect.right(), rect.top() + 2);
        }
        else
        {
            //painter->setPen(this->mColorOdd);
            QPen pen(QBrush(this->mColorOdd), 3);
            painter->setPen(pen);
            painter->drawLine(rect.left(), rect.bottom() - 2, rect.right(), rect.bottom() - 2);
        }
    }
    QItemDelegate::paint(painter, option, index);
}
//-----------------------------------------------------------------------------
int ByteListItemDelegate::bitsCount()
{
    return this->mBitsCount;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setBitsCount(const int value)
{
    this->mBitsCount = value;
}
//-----------------------------------------------------------------------------
QColor ByteListItemDelegate::colorOdd() const
{
    return this->mColorOdd;
}
//-----------------------------------------------------------------------------
QColor ByteListItemDelegate::colorEven() const
{
    return this->mColorEven;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setColorOdd(const QColor &value)
{
    this->mColorOdd = value;
}
//-----------------------------------------------------------------------------
void ByteListItemDelegate::setColorEven(const QColor &value)
{
    this->mColorEven = value;
}
//-----------------------------------------------------------------------------
