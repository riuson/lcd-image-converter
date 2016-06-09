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

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <QObject>

#include <QMap>
#include <QStringList>
#include <QString>
#include <QStringList>
#include <QVariant>
//-----------------------------------------------------------------------------
class QImage;
class HistoryKeeper;
//-----------------------------------------------------------------------------
class DataContainer : public QObject
{
    Q_OBJECT
public:
    explicit DataContainer(QObject *parent = 0);
    virtual ~DataContainer();

    const QImage *image(const QString &imageKey) const;
    void setImage(const QString &imageKey, const QImage *image);

    QVariant commonInfo(const QString &infoKey) const;
    void setCommonInfo(const QString &infoKey, const QVariant &value);

    QVariant imageInfo(const QString &imageKey, const QString &infoKey) const;
    void setImageInfo(const QString &imageKey, const QString &infoKey, const QVariant &value);

    void clear();
    int count() const;
    QStringList keys() const;
    void removeImage(const QString &key);

    void reorderTo(const QStringList *keys);

    bool historyInitialized() const;
    void historyInit();
    void stateSave();
    void stateUndo();
    void stateRedo();
    bool canUndo() const;
    bool canRedo() const;

    bool changed() const;
    void setChanged(bool value);

private:
    static const QString DataChangedKey;
    static const QString CommonInfoKeyPrefix;
    static const QString ImageInfoKeyPrefix;

    QMap<QString, QImage *> mImageMap;
    QMap<QString, QVariant> mInfoMap;
    QStringList mKeys;
    QImage *mDefaultImage;
    HistoryKeeper *mHistory;

signals:
    void dataChanged(bool historyStateMoved);
};
//-----------------------------------------------------------------------------
#endif // DATACONTAINER_H
