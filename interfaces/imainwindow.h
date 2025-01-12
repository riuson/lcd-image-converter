/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#ifndef IMAINWINDOW_H
#define IMAINWINDOW_H

#include <QObject>
#include <QString>
#include <QWidget>

template <class T> class QList;

namespace AppUI
{
class IEditor;

class IMainWindow
{
public:
  virtual IEditor* currentEditor() = 0;
  virtual QWidget* currentTab() = 0;
  virtual void tabsList(QList<QWidget*>* list) = 0;
  virtual QWidget* parentWidget() = 0;
  virtual QString findAvailableName(const QString& prefix) = 0;
};
} // namespace AppUI

Q_DECLARE_INTERFACE(AppUI::IMainWindow, "riuson.lcd-image-converter/1.0")

#endif // IDOCUMENT_H
