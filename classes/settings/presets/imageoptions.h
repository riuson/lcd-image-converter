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

#ifndef IMAGEOPTIONS_H
#define IMAGEOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
class QSettings;
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class ImageOptions : public QObject
{
    Q_OBJECT
public:
    explicit ImageOptions(QObject *parent = 0);

    bool splitToRows() const;
    BytesOrder bytesOrder() const;
    DataBlockSize blockSize() const;
    bool blockDefaultOnes() const;
    bool compressionRle() const;
    quint32 compressionRleMinLength() const;
    QString blockPrefix() const;
    QString blockSuffix() const;
    QString blockDelimiter() const;

    void setSplitToRows(bool value);
    void setBytesOrder(BytesOrder value);
    void setBlockSize(DataBlockSize value);
    void setBlockDefaultOnes(bool value);
    void setCompressionRle(bool value);
    void setCompressionRleMinLength(quint32 value);
    void setBlockPrefix(const QString &value);
    void setBlockSuffix(const QString &value);
    void setBlockDelimiter(const QString &value);

    bool load(QSettings *settings, int version);
    void save(QSettings *settings);

private:
    static const QString GroupName;
    static const QString FieldBytesOrder;
    static const QString FieldBlockSize;
    static const QString FieldBlockDefaultOnes;
    static const QString FieldSplitToRows;
    static const QString FieldCompressionRle;
    static const QString FieldCompressionRleMinLength;
    static const QString FieldBlockPrefix;
    static const QString FieldBandWidth;
    static const QString FieldBlockSuffix;
    static const QString FieldBlockDelimiter;

    bool mSplitToRows;
    BytesOrder mBytesOrder;
    DataBlockSize mBlockSize;
    bool mBlockDefaultOnes;
    bool mCompressionRle;
    quint32 mCompressionRleMinLength;
    QString mBlockPrefix;
    QString mBlockSuffix;
    QString mBlockDelimiter;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // IMAGEOPTIONS_H
