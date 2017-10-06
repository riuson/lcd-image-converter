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

#ifndef HISTORYKEEPER_H
#define HISTORYKEEPER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QVariant>
#include <QList>
#include <QMap>
#include <QStringList>

class HistoryRecord;

class HistoryKeeper : public QObject
{
  Q_OBJECT
public:
  explicit HistoryKeeper(QObject *parent = 0);
  virtual ~HistoryKeeper();

  void init(
    const QStringList *keys,
    const QMap<QString, QImage *> *images,
    const QMap<QString, QVariant> *info);
  void store(
    const QStringList *keys,
    const QMap<QString, QImage *> *images,
    const QMap<QString, QVariant> *info);
  void restorePrevious(
    QStringList *keys,
    QMap<QString, QImage *> *images,
    QMap<QString, QVariant> *info);
  void restoreNext(
    QStringList *keys,
    QMap<QString, QImage *> *images,
    QMap<QString, QVariant> *info);

  bool initialized() const;
  bool canRestorePrevious() const;
  bool canRestoreNext() const;

private:
  QList<HistoryRecord *> *mHistory;
  int mCurrentIndex;

  void removeAfter(int index);
  void restoreAt(
    int index,
    QStringList *keys,
    QMap<QString, QImage *> *images,
    QMap<QString, QVariant> *info);
};

#endif // HISTORYKEEPER_H
