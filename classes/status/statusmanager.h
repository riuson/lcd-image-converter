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

#ifndef STATUSMANAGER_H
#define STATUSMANAGER_H

#include <QObject>
#include <QMap>
#include "statusdata.h"

class QStatusBar;
class QLabel;

namespace AppUI
{
namespace Status
{

class StatusManager : public QObject
{
  Q_OBJECT
public:
  explicit StatusManager(QStatusBar *statusBar, QObject *parent = 0);
  virtual ~StatusManager();

  void updateData(const StatusData *statuses);
  void hideAll();

private:
  QStatusBar *mBar;
  QMap<StatusData::StatusType, QLabel *> mList;

  void updateItem(StatusData::StatusType key, const StatusData *statuses);
  void addItem(StatusData::StatusType key);
  void hideItem(StatusData::StatusType key);

signals:

public slots:

};

} // namespace Status
} // namespace AppUI

#endif // STATUSMANAGER_H
