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

#include "reorderingoptions.h"
//-----------------------------------------------------------------------------
#include <QVector>
#include <QSettings>
//-----------------------------------------------------------------------------
const QString ReorderingOptions::GroupName = QString("reordering");
const QString ReorderingOptions::FieldOperations = QString("operations");
const QString ReorderingOptions::FieldMask = QString("mask");
const QString ReorderingOptions::FieldShift = QString("shift");
const QString ReorderingOptions::FieldLeft = QString("left");
//-----------------------------------------------------------------------------
ReorderingOptions::ReorderingOptions(QObject *parent) :
    QObject(parent)
{
    this->mOperations = new QVector<quint32>();
}
//-----------------------------------------------------------------------------
ReorderingOptions::~ReorderingOptions()
{
    delete this->mOperations;
}
//-----------------------------------------------------------------------------
int ReorderingOptions::operationsCount() const
{
    return (this->mOperations->size()) / 2;
}
//-----------------------------------------------------------------------------
void ReorderingOptions::operation(int index, quint32 *mask, int *shift, bool *left) const
{
    *mask = 0;
    *shift = 0;
    *left = false;

    if (index < this->operationsCount())
    {
        index = (index * 2);

        *mask = this->mOperations->at(index);
        *shift = (this->mOperations->at(index + 1) & 0x0000001f);
        *left = (this->mOperations->at(index + 1) & 0x80000000) != 0;
    }
}
//-----------------------------------------------------------------------------
void ReorderingOptions::operationAdd(quint32 mask, int shift, bool left)
{
    shift = qAbs(shift);

    this->mOperations->append(mask);
    if (left)
        this->mOperations->append(shift | 0x80000000);
    else
        this->mOperations->append(shift);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ReorderingOptions::operationRemove(int index)
{
    if (index < this->operationsCount())
    {
        index *= 2;
        this->mOperations->remove(index + 1);
        this->mOperations->remove(index);
    }

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ReorderingOptions::operationsRemoveAll()
{
    for (int i = this->operationsCount() - 1; i >= 0; i--)
        this->operationRemove(i);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ReorderingOptions::operationReplace(int index, quint32 mask, int shift, bool left)
{
    if (index < this->operationsCount())
    {
        index *= 2;

        this->mOperations->replace(index, mask);

        if (left)
            this->mOperations->replace(index + 1, shift | 0x80000000);
        else
            this->mOperations->replace(index + 1, shift);
    }

    emit this->changed();
}
//-----------------------------------------------------------------------------
bool ReorderingOptions::load(QSettings *settings, int version)
{
    bool result = true;// this option not implemented in versions < 2013-01-04

    if (version == 2)
    {
        settings->beginGroup(ReorderingOptions::GroupName);

        this->operationsRemoveAll();

        int iOperations = settings->beginReadArray(ReorderingOptions::FieldOperations);
        for (int i = 0; i < iOperations; i++)
        {
            settings->setArrayIndex(i);

            QString sMask = settings->value(ReorderingOptions::FieldMask, QString("00000000")).toString();
            quint32 uMask, uShift, uLeft;

            if (result)
                uMask = sMask.toUInt(&result, 16);

            if (result)
                uShift = settings->value(ReorderingOptions::FieldShift, uint(0)).toUInt(&result);

            if (result)
                uLeft = settings->value(ReorderingOptions::FieldLeft, uint(0)).toUInt(&result);

            if (result)
            {
                this->operationAdd(uMask, uShift, uLeft != 0);
            }
        }
        settings->endArray();

        settings->endGroup();
    }

    return result;
}
//-----------------------------------------------------------------------------
void ReorderingOptions::save(QSettings *settings)
{
    settings->beginGroup(ReorderingOptions::GroupName);

    settings->beginWriteArray(ReorderingOptions::FieldOperations);

    for (int i = 0; i < this->operationsCount(); i++)
    {
        quint32 uMask;
        int iShift;
        bool bLeft;
        this->operation(i, &uMask, &iShift, &bLeft);

        settings->setArrayIndex(i);
        settings->setValue(ReorderingOptions::FieldMask,  QString("%1").arg(uMask, 8, 16, QChar('0')));
        settings->setValue(ReorderingOptions::FieldShift, QString("%1").arg(iShift));
        settings->setValue(ReorderingOptions::FieldLeft,  QString("%1").arg((int)bLeft));
    }
    settings->endArray();

    settings->endGroup();
}
//-----------------------------------------------------------------------------
