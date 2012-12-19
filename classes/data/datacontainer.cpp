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

#include "datacontainer.h"

#include <QImage>
#include "historykeeper.h"
//-----------------------------------------------------------------------------
DataContainer::DataContainer(QObject *parent) :
    QObject(parent)
{
    this->mDefaultImage = new QImage(":/images/template");
    this->mHistory = new HistoryKeeper(this);
}
//-----------------------------------------------------------------------------
DataContainer::~DataContainer()
{
    qDeleteAll(this->mImageMap);
    delete this->mDefaultImage;

    delete this->mHistory;
}
//-----------------------------------------------------------------------------
const QImage *DataContainer::image(const QString &key) const
{
    return this->mImageMap.value(key, this->mDefaultImage);
}
//-----------------------------------------------------------------------------
void DataContainer::setImage(const QString &key, const QImage *image)
{
    this->remove(key);
    QImage *imageNew = new QImage(*image);
    this->mImageMap.insert(key, imageNew);
    emit this->imageChanged(key);
}
//-----------------------------------------------------------------------------
QVariant DataContainer::info(const QString &key) const
{
    if (this->mInfoMap.contains(key))
    {
        return this->mInfoMap.value(key);
    }
    return QVariant();
}
//-----------------------------------------------------------------------------
void DataContainer::setInfo(const QString &key, const QVariant &value)
{
    this->mInfoMap.insert(key, value);
}
//-----------------------------------------------------------------------------
void DataContainer::clear()
{
    qDeleteAll(this->mImageMap);
    this->mImageMap.clear();
}
//-----------------------------------------------------------------------------
int DataContainer::count() const
{
    return this->mImageMap.count();
}
//-----------------------------------------------------------------------------
QStringList DataContainer::keys() const
{
    QList<QString> tmp = this->mImageMap.keys();
    qSort(tmp);
    QStringList result(tmp);
    return result;
}
//-----------------------------------------------------------------------------
void DataContainer::remove(const QString &key)
{
    if (this->mImageMap.contains(key))
    {
        QImage *imageOld = this->mImageMap.value(key);
        this->mImageMap.remove(key);
        delete imageOld;
    }
}
//-----------------------------------------------------------------------------
void DataContainer::stateSave()
{
    this->mHistory->store(&this->mImageMap, &this->mInfoMap);
}
//-----------------------------------------------------------------------------
void DataContainer::stateUndo()
{
    this->mHistory->restorePrevious(&this->mImageMap, &this->mInfoMap);
}
//-----------------------------------------------------------------------------
void DataContainer::stateRedo()
{
    this->mHistory->restoreNext(&this->mImageMap, &this->mInfoMap);
}
//-----------------------------------------------------------------------------
bool DataContainer::canUndo() const
{
    return this->mHistory->canRestorePrevious();
}
//-----------------------------------------------------------------------------
bool DataContainer::canRedo() const
{
    return this->mHistory->canRestoreNext();
}
//-----------------------------------------------------------------------------
