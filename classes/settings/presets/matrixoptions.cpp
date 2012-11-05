/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "matrixoptions.h"
//-----------------------------------------------------------------------------
#include <QVector>
#include <QSettings>
//-----------------------------------------------------------------------------
MatrixOptions::MatrixOptions(QObject *parent) :
    QObject(parent)
{
    this->mOperations = new QVector<quint32>();
    this->mMaskUsed = 0x00ffffff;
    this->mMaskAnd = 0xffffffff;
    this->mMaskOr = 0x00000000;
    this->mMaskFill = 0xffffffff;
}
//-----------------------------------------------------------------------------
MatrixOptions::~MatrixOptions()
{
    delete this->mOperations;
}
//-----------------------------------------------------------------------------
quint32 MatrixOptions::maskUsed() const
{
    return this->mMaskUsed;
}
//-----------------------------------------------------------------------------
quint32 MatrixOptions::maskAnd() const
{
    return this->mMaskAnd;
}
//-----------------------------------------------------------------------------
quint32 MatrixOptions::maskOr() const
{
    return this->mMaskOr;
}
//-----------------------------------------------------------------------------
quint32 MatrixOptions::maskFill() const
{
    return this->mMaskFill;
}
//-----------------------------------------------------------------------------
void MatrixOptions::setMaskUsed(quint32 value)
{
    this->mMaskUsed = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void MatrixOptions::setMaskAnd(quint32 value)
{
    this->mMaskAnd = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void MatrixOptions::setMaskOr(quint32 value)
{
    this->mMaskOr = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void MatrixOptions::setMaskFill(quint32 value)
{
    if (value == 0)
        value = 0xffffffff;

    this->mMaskFill = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
int MatrixOptions::operationsCount() const
{
    return (this->mOperations->size()) / 2;
}
//-----------------------------------------------------------------------------
void MatrixOptions::operation(int index, quint32 *mask, int *shift, bool *left) const
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
void MatrixOptions::operationAdd(quint32 mask, int shift, bool left)
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
void MatrixOptions::operationRemove(int index)
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
void MatrixOptions::operationsRemoveAll()
{
    for (int i = this->operationsCount() - 1; i >= 0; i--)
        this->operationRemove(i);

    emit this->changed();
}
//-----------------------------------------------------------------------------
void MatrixOptions::operationReplace(int index, quint32 mask, int shift, bool left)
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
bool MatrixOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        quint32 uMaskUsed = 0, uMaskAnd = 0, uMaskOr = 0, uMaskFill = 0;

        QString sMaskUsed = settings->value("maskUsed", QString("ffffffff")).toString();
        QString sMaskAnd  = settings->value("maskAnd",  QString("ffffffff")).toString();
        QString sMaskOr   = settings->value("maskOr",   QString("00000000")).toString();
        QString sMaskFill = settings->value("maskFill", QString("ffffffff")).toString();

        uMaskUsed = sMaskUsed.toUInt(&result, 16);

        if (result)
            uMaskAnd = sMaskAnd.toUInt(&result, 16);

        if (result)
            uMaskOr = sMaskOr.toUInt(&result, 16);

        if (result)
            uMaskFill = sMaskFill.toUInt(&result, 16);

        if (result)
        {
            this->setMaskUsed(uMaskUsed);
            this->setMaskAnd(uMaskAnd);
            this->setMaskOr(uMaskOr);
            this->setMaskFill(uMaskFill);

            this->operationsRemoveAll();

            int iOperations = settings->beginReadArray("matrix");
            for (int i = 0; i < iOperations; i++)
            {
                settings->setArrayIndex(i);

                QString sMask = settings->value("mask", QString("00000000")).toString();
                quint32 uMask, uShift, uLeft;

                if (result)
                    uMask = sMask.toUInt(&result, 16);

                if (result)
                    uShift = settings->value("shift", uint(0)).toUInt(&result);

                if (result)
                    uLeft = settings->value("left", uint(0)).toUInt(&result);

                if (result)
                {
                    this->operationAdd(uMask, uShift, uLeft != 0);
                }
            }
            settings->endArray();
        }
    }
    else if (version == 2)
    {
        settings->beginGroup("matrix");

        quint32 uMaskUsed = 0, uMaskAnd = 0, uMaskOr = 0, uMaskFill = 0;

        QString sMaskUsed = settings->value("maskUsed", QString("ffffffff")).toString();
        QString sMaskAnd  = settings->value("maskAnd",  QString("ffffffff")).toString();
        QString sMaskOr   = settings->value("maskOr",   QString("00000000")).toString();
        QString sMaskFill = settings->value("maskFill", QString("ffffffff")).toString();

        uMaskUsed = sMaskUsed.toUInt(&result, 16);

        if (result)
            uMaskAnd = sMaskAnd.toUInt(&result, 16);

        if (result)
            uMaskOr = sMaskOr.toUInt(&result, 16);

        if (result)
            uMaskFill = sMaskFill.toUInt(&result, 16);

        if (result)
        {
            this->setMaskUsed(uMaskUsed);
            this->setMaskAnd(uMaskAnd);
            this->setMaskOr(uMaskOr);
            this->setMaskFill(uMaskFill);

            this->operationsRemoveAll();

            int iOperations = settings->beginReadArray("operations");
            for (int i = 0; i < iOperations; i++)
            {
                settings->setArrayIndex(i);

                QString sMask = settings->value("mask", QString("00000000")).toString();
                quint32 uMask, uShift, uLeft;

                if (result)
                    uMask = sMask.toUInt(&result, 16);

                if (result)
                    uShift = settings->value("shift", uint(0)).toUInt(&result);

                if (result)
                    uLeft = settings->value("left", uint(0)).toUInt(&result);

                if (result)
                {
                    this->operationAdd(uMask, uShift, uLeft != 0);
                }
            }
            settings->endArray();
        }

        settings->endGroup();
    }

    return result;
}
//-----------------------------------------------------------------------------
void MatrixOptions::save(QSettings *settings)
{
    settings->beginGroup("matrix");

    settings->setValue("maskUsed", QString("%1").arg(this->maskUsed(), 8, 16, QChar('0')));
    settings->setValue("maskAnd",  QString("%1").arg(this->maskAnd(),  8, 16, QChar('0')));
    settings->setValue("maskOr",   QString("%1").arg(this->maskOr(),   8, 16, QChar('0')));
    settings->setValue("maskFill", QString("%1").arg(this->maskFill(), 8, 16, QChar('0')));

    settings->beginWriteArray("operations");

    for (int i = 0; i < this->operationsCount(); i++)
    {
        quint32 uMask;
        int iShift;
        bool bLeft;
        this->operation(i, &uMask, &iShift, &bLeft);

        settings->setArrayIndex(i);
        settings->setValue("mask",  QString("%1").arg(uMask, 8, 16, QChar('0')));
        settings->setValue("shift", QString("%1").arg(iShift));
        settings->setValue("left",  QString("%1").arg((int)bLeft));
    }
    settings->endArray();

    settings->endGroup();
}
//-----------------------------------------------------------------------------
