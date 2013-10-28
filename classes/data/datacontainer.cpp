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
const QString DataContainer::DataChangedKey = QString("data changed");
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
    this->setChanged(true);

    emit this->imagesChanged();
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
    //TODO: may be need to compare old and new values?
    this->mInfoMap.insert(key, value);
    this->setChanged(true);
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
bool DataContainer::historyInitialized() const
{
    return this->mHistory->initialized();
}
//-----------------------------------------------------------------------------
void DataContainer::historyInit()
{
    this->mHistory->init(&this->mImageMap, &this->mInfoMap);
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

    emit this->imagesChanged();
}
//-----------------------------------------------------------------------------
void DataContainer::stateRedo()
{
    this->mHistory->restoreNext(&this->mImageMap, &this->mInfoMap);

    emit this->imagesChanged();
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
bool DataContainer::changed() const
{
    QVariant value = this->info(DataContainer::DataChangedKey);
    if (!value.isNull())
    {
        return value.toBool();
    }
    return false;
}
//-----------------------------------------------------------------------------
void DataContainer::setChanged(bool value)
{
    this->mInfoMap.insert(DataContainer::DataChangedKey, value);
}
//-----------------------------------------------------------------------------
