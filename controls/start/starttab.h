/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef STARTTAB_H
#define STARTTAB_H

#include <QWidget>

namespace Ui
{
class StartTab;
}

class QStringList;

namespace AppUI
{
namespace Start
{

class StartTab : public QWidget
{
  Q_OBJECT

public:
  explicit StartTab(QWidget *parent = 0);
  virtual ~StartTab();

  void setRecentFiles(const QStringList *list);
  const QString tabName() const;

protected:
  void changeEvent(QEvent *e) Q_DECL_OVERRIDE;

private:
  Ui::StartTab *ui;
  const QStringList *mRecentFilesList;

signals:
  void openRecent(const QString &filename);
  void createNewImage();
  void createNewFont();
};

} // namespace Start
} // namespace AppUI

#endif // STARTTAB_H
