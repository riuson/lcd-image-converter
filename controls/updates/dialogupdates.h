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

#ifndef DIALOGUPDATES_H
#define DIALOGUPDATES_H

#include <QDialog>

namespace Ui
{
class DialogUpdates;
}

class QNetworkReply;

class DialogUpdates : public QDialog
{
  Q_OBJECT

public:
  explicit DialogUpdates(QWidget *parent = 0);
  virtual ~DialogUpdates();

private:
  Ui::DialogUpdates *ui;

  void showHistory();
  void showUpdates();
  void showUpdates(const QString &xml);
  void showError(const QString &message);
  bool transformHistory(const QString &xml, const QString &xsl, QString *html);
  bool isLocalVersionOutdated(const QString &xml);

private slots:
  void networkReply(QNetworkReply *reply);
};

#endif // DIALOGUPDATES_H
