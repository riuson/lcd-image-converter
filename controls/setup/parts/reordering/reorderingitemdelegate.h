/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#ifndef REORDERINGITEMDELEGATE_H
#define REORDERINGITEMDELEGATE_H

#include <QtGui>
#include <QtWidgets>

namespace AppUI
{
namespace Setup
{
namespace Parts
{
namespace Reordering
{

class ReorderingItemDelegate : public QItemDelegate
{
  Q_OBJECT
public:
  explicit ReorderingItemDelegate(QObject *parent = 0);
  virtual ~ReorderingItemDelegate() {}

  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

public:
  QColor color() const;
  void setColor(const QColor &value);
private:
  QColor mColor;
};

} // namespace Reordering
} // namespace Parts
} // namespace Setup
} // namespace AppUI

#endif // REORDERINGITEMDELEGATE_H
