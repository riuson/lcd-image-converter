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

#include "historykeeper.h"

#include <QStringList>
#include <QStringListIterator>
#include <QDebug>
#include "historyrecord.h"

namespace Data
{
namespace History
{

HistoryKeeper::HistoryKeeper(QObject *parent) :
  QObject(parent)
{
  this->mHistory = new QList<HistoryRecord *>();
  this->mCurrentIndex = -1;
}

HistoryKeeper::~HistoryKeeper()
{
  qDeleteAll(*this->mHistory);
  this->mHistory->clear();
}

void HistoryKeeper::init(const QStringList *keys, const QMap<QString, QImage *> *images, const QMap<QString, QVariant> *info)
{
  // clear all
  this->removeAfter(-1);

  // initialize by current state
  HistoryRecord *record = new HistoryRecord(keys, images, info, this);
  this->mHistory->append(record);

  // first recorded state
  this->mCurrentIndex = 0;
}

void HistoryKeeper::store(
  const QStringList *keys,
  const QMap<QString, QImage *> *images,
  const QMap<QString, QVariant> *info)
{
  if (this->mCurrentIndex < 0) {
    qDebug() << "history keeper not initialized";
    return;
  }

  this->removeAfter(this->mCurrentIndex);

  HistoryRecord *record = new HistoryRecord(keys, images, info, this);
  this->mHistory->append(record);
  this->mCurrentIndex++;
}

void HistoryKeeper::restorePrevious(
  QStringList *keys,
  QMap<QString, QImage *> *images,
  QMap<QString, QVariant> *info)
{
  if (this->mCurrentIndex < 0) {
    qDebug() << "history keeper not initialized";
    return;
  }

  if (this->canRestorePrevious()) {
    this->mCurrentIndex--;
    this->restoreAt(this->mCurrentIndex, keys, images, info);
  }
}

void HistoryKeeper::restoreNext(
  QStringList *keys,
  QMap<QString, QImage *> *images,
  QMap<QString, QVariant> *info)
{
  if (this->mCurrentIndex < 0) {
    qDebug() << "history keeper not initialized";
    return;
  }

  if (this->canRestoreNext()) {
    this->mCurrentIndex++;
    this->restoreAt(this->mCurrentIndex, keys, images, info);
  }
}

bool HistoryKeeper::initialized() const
{
  return (this->mCurrentIndex >= 0);
}

bool HistoryKeeper::canRestorePrevious() const
{
  return (this->mCurrentIndex > 0);
}

bool HistoryKeeper::canRestoreNext() const
{
  return (this->mCurrentIndex < (this->mHistory->length() - 1));
}

void HistoryKeeper::removeAfter(int index)
{
  if (index < -1) {
    index = -1;
  }

  for (int i = this->mHistory->length() - 1; i > index; i--) {
    HistoryRecord *record = this->mHistory->at(i);
    this->mHistory->removeAt(i);
    delete record;
  }
}

void HistoryKeeper::restoreAt(
  int index,
  QStringList *keys,
  QMap<QString, QImage *> *images,
  QMap<QString, QVariant> *info)
{
  HistoryRecord *record = this->mHistory->at(index);

  qDeleteAll(*images);
  keys->clear();
  images->clear();
  info->clear();

  keys->append(*record->keys());
  QStringListIterator iterator(*keys);

  while (iterator.hasNext()) {
    QString key = iterator.next();

    QImage *oldImage = record->images()->value(key);
    QImage *newImage = new QImage(*oldImage);

    images->insert(key, newImage);
  }

  iterator = QStringListIterator(record->info()->keys());

  while (iterator.hasNext()) {
    QString key = iterator.next();

    QVariant value = record->info()->value(key);

    info->insert(key, value);
  }
}

} // namespace Containers
} // namespace Data
