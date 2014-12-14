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
    this->mKeys.clear();
    delete this->mDefaultImage;

    delete this->mHistory;
}
//-----------------------------------------------------------------------------
const QImage *DataContainer::image(const QString &key) const
{
    if (this->mKeys.contains(key))
    {
        return this->mImageMap.value(key, this->mDefaultImage);
    }
    else
    {
        return this->mDefaultImage;
    }
}
//-----------------------------------------------------------------------------
void DataContainer::setImage(const QString &key, const QImage *image)
{
    if (this->mKeys.contains(key))
    {
        this->mImageMap.remove(key);
    }
    else
    {
        this->mKeys.append(key);
    }
    QImage *imageNew = new QImage(*image);
    this->mImageMap.insert(key, imageNew);

    this->setChanged(true);
    emit this->dataChanged(false);
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
    bool changed = false;

    //TODO: may be need to compare old and new values?
    if (this->mInfoMap.contains(key))
    {
        if (this->mInfoMap.value(key) != value)
        {
            changed = true;
        }
    }
    else
    {
        changed = true;
    }

    if (changed)
    {
        this->mInfoMap.insert(key, value);
        this->setChanged(true);
        emit this->dataChanged(false);
    }
}
//-----------------------------------------------------------------------------
void DataContainer::clear()
{
    qDeleteAll(this->mImageMap);
    this->mKeys.clear();
    this->mImageMap.clear();
}
//-----------------------------------------------------------------------------
int DataContainer::count() const
{
    return this->mKeys.count();
}
//-----------------------------------------------------------------------------
QStringList DataContainer::keys() const
{
    QStringList result(this->mKeys);
    return result;
}
//-----------------------------------------------------------------------------
void DataContainer::removeImage(const QString &key)
{
    if (this->mKeys.contains(key))
    {
        QImage *imageOld = this->mImageMap.value(key);
        this->mImageMap.remove(key);
        this->mKeys.removeOne(key);
        delete imageOld;

        this->setChanged(true);
        emit this->dataChanged(false);
    }
}
//-----------------------------------------------------------------------------
void DataContainer::reorderTo(const QStringList *keys)
{
    if (this->mKeys.length() == keys->length())
    {
        // all 'keys' contains in 'mKeys'
        bool exists = true;
        for (int i = 0; i < keys->length(); i++)
        {
            QString key = keys->at(i);
            if (!this->mKeys.contains(key))
            {
                exists = false;
                break;
            }
        }

        if (exists)
        {
            this->mKeys.clear();
            this->mKeys.append(*keys);
        }
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
    this->mHistory->init(&this->mKeys, &this->mImageMap, &this->mInfoMap);
}
//-----------------------------------------------------------------------------
void DataContainer::stateSave()
{
    this->mHistory->store(&this->mKeys, &this->mImageMap, &this->mInfoMap);
}
//-----------------------------------------------------------------------------
void DataContainer::stateUndo()
{
    this->mHistory->restorePrevious(&this->mKeys, &this->mImageMap, &this->mInfoMap);
    this->setChanged(true);

    emit this->dataChanged(true);
}
//-----------------------------------------------------------------------------
void DataContainer::stateRedo()
{
    this->mHistory->restoreNext(&this->mKeys, &this->mImageMap, &this->mInfoMap);
    this->setChanged(true);

    emit this->dataChanged(true);
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
