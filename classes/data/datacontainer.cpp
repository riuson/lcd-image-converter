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
const QString DataContainer::CommonInfoKeyPrefix = QString("common:");
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
const QImage *DataContainer::image(const QString &imageKey) const
{
    if (this->mKeys.contains(imageKey))
    {
        return this->mImageMap.value(imageKey, this->mDefaultImage);
    }
    else
    {
        return this->mDefaultImage;
    }
}
//-----------------------------------------------------------------------------
void DataContainer::setImage(const QString &imageKey, const QImage *image)
{
    if (this->mKeys.contains(imageKey))
    {
        this->mImageMap.remove(imageKey);
    }
    else
    {
        this->mKeys.append(imageKey);
    }
    QImage *imageNew = new QImage(*image);
    this->mImageMap.insert(imageKey, imageNew);

    this->setChanged(true);
    emit this->dataChanged(false);
}
//-----------------------------------------------------------------------------
QVariant DataContainer::commonInfo(const QString &infoKey) const
{
    if (this->mInfoMap.contains(DataContainer::CommonInfoKeyPrefix + infoKey))
    {
        return this->mInfoMap.value(DataContainer::CommonInfoKeyPrefix + infoKey);
    }
    return QVariant();
}
//-----------------------------------------------------------------------------
void DataContainer::setCommonInfo(const QString &infoKey, const QVariant &value)
{
    bool changed = false;

    //TODO: may be need to compare old and new values?
    if (this->mInfoMap.contains(DataContainer::CommonInfoKeyPrefix + infoKey))
    {
        if (this->mInfoMap.value(DataContainer::CommonInfoKeyPrefix + infoKey) != value)
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
        this->mInfoMap.insert(DataContainer::CommonInfoKeyPrefix + infoKey, value);
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
    if (this->mInfoMap.contains(DataContainer::DataChangedKey))
    {
        QVariant value = this->mInfoMap.value(DataContainer::DataChangedKey, QVariant(false));

        if (!value.isNull())
        {
            return value.toBool();
        }
    }

    return false;
}
//-----------------------------------------------------------------------------
void DataContainer::setChanged(bool value)
{
    this->mInfoMap.insert(DataContainer::DataChangedKey, value);
}
//-----------------------------------------------------------------------------
