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

#ifndef PREPAREOPTIONS_H
#define PREPAREOPTIONS_H
//-----------------------------------------------------------------------------
#include <QObject>
//-----------------------------------------------------------------------------
#include "conversion_options.h"
//-----------------------------------------------------------------------------
using namespace ConversionOptions;
//-----------------------------------------------------------------------------
class PrepareOptions : public QObject
{
    Q_OBJECT
public:
    explicit PrepareOptions(QObject *parent = 0);

    ConversionType convType() const;
    MonochromeType monoType() const;
    int edge() const;
    ScanMainDirection scanMain() const;
    ScanSubDirection scanSub() const;
    bool inverse() const;

    void setConvType(ConversionType value);
    void setMonoType(MonochromeType value);
    void setEdge(int value);
    void setScanMain(ScanMainDirection value);
    void setScanSub(ScanSubDirection value);
    void setInverse(bool value);

    const QString & convTypeName() const;
    const QString & monoTypeName() const;

    void modificationsFromScan(
            Rotate *rotate,
            bool *flipHorizontal,
            bool *flipVertical) const;

private:
    ConversionType mConvType;
    MonochromeType mMonoType;
    int mEdge;
    ScanMainDirection mScanMain;
    ScanSubDirection mScanSub;
    bool mInverse;

signals:
    void changed();

public slots:

};
//-----------------------------------------------------------------------------
#endif // PREPAREOPTIONS_H
