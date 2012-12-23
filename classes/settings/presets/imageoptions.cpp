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

#include "imageoptions.h"
//-----------------------------------------------------------------------------
#include <QSettings>
//-----------------------------------------------------------------------------
ImageOptions::ImageOptions(QObject *parent) :
    QObject(parent)
{
    this->mSplitToRows = true;
    this->mBytesOrder = BytesOrderLittleEndian;
    this->mBlockSize = Data8;
    this->mCompressionRle = false;
}
//-----------------------------------------------------------------------------
bool ImageOptions::splitToRows() const
{
    return this->mSplitToRows;
}
//-----------------------------------------------------------------------------
BytesOrder ImageOptions::bytesOrder() const
{
    return this->mBytesOrder;
}
//-----------------------------------------------------------------------------
DataBlockSize ImageOptions::blockSize() const
{
    if (this->mBlockSize <= Data32)
    {
        return this->mBlockSize;
    }
    return Data32;
}
//-----------------------------------------------------------------------------
bool ImageOptions::compressionRle() const
{
    return this->mCompressionRle;
}
//-----------------------------------------------------------------------------
void ImageOptions::setSplitToRows(bool value)
{
    this->mSplitToRows = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ImageOptions::setBytesOrder(BytesOrder value)
{
    if (value < BytesOrderLittleEndian || value > BytesOrderBigEndian)
        value = BytesOrderLittleEndian;
    this->mBytesOrder = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ImageOptions::setBlockSize(DataBlockSize value)
{
    if (value < Data8 || value > Data32)
        value = Data32;
    this->mBlockSize = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void ImageOptions::setCompressionRle(bool value)
{
    this->mCompressionRle = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
bool ImageOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        quint32 uBytesOrder = 0, uBlockSize = 0, uSplitToRows = 0, uCompressionRle = 0;

        uBlockSize = settings->value("blockSize", int(0)).toUInt(&result);

        if (result)
            uBytesOrder = settings->value("bytesOrder", int(0)).toUInt(&result);

        if (result)
            uSplitToRows = settings->value("splitToRows", int(1)).toUInt(&result);

        if (result)
            uCompressionRle = settings->value("compressionRle", int(0)).toUInt(&result);

        if (result)
        {
            this->setBlockSize((DataBlockSize)uBlockSize);
            this->setBytesOrder((BytesOrder)uBytesOrder);
            this->setSplitToRows((bool)uSplitToRows);
            this->setCompressionRle((bool)uCompressionRle);
        }
    }
    else if (version == 2)
    {
        settings->beginGroup("image");

        result = this->load(settings, 1);

        settings->endGroup();
    }

    return result;
}
//-----------------------------------------------------------------------------
void ImageOptions::save(QSettings *settings)
{
    settings->beginGroup("image");

    settings->setValue("bytesOrder",     QString("%1").arg((int)this->bytesOrder()));
    settings->setValue("blockSize",      QString("%1").arg((int)this->blockSize()));
    settings->setValue("splitToRows",    QString("%1").arg((int)this->splitToRows()));
    settings->setValue("compressionRle", QString("%1").arg((int)this->compressionRle()));

    settings->endGroup();
}
//-----------------------------------------------------------------------------
