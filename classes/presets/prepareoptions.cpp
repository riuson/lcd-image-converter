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

#include "prepareoptions.h"
//-----------------------------------------------------------------------------
#include <QSettings>
//-----------------------------------------------------------------------------
PrepareOptions::PrepareOptions(QObject *parent) :
    QObject(parent)
{
    this->mConvType = ConversionTypeMonochrome;
    this->mMonoType = MonochromeTypeDiffuseDither;
    this->mEdge = 128;
    this->mScanMain = TopToBottom;
    this->mScanSub = Forward;
    this->mInverse = false;
}
//-----------------------------------------------------------------------------
ConversionType PrepareOptions::convType() const
{
    if (this->mConvType <= ConversionTypeColor)
    {
        return this->mConvType;
    }
    return ConversionTypeColor;
}
//-----------------------------------------------------------------------------
MonochromeType PrepareOptions::monoType() const
{
    if (this->mMonoType <= MonochromeTypeThresholdDither)
    {
        return this->mMonoType;
    }
    return MonochromeTypeThresholdDither;
}
//-----------------------------------------------------------------------------
int PrepareOptions::edge() const
{
    if (this->mEdge <= 255)
    {
        return this->mEdge;
    }
    return 128;
}
//-----------------------------------------------------------------------------
ScanMainDirection PrepareOptions::scanMain() const
{
    if (this->mScanMain <= RightToLeft)
    {
        return this->mScanMain;
    }
    return TopToBottom;
}
//-----------------------------------------------------------------------------
ScanSubDirection PrepareOptions::scanSub() const
{
    if (this->mScanSub <= Backward)
    {
        return this->mScanSub;
    }
    return Forward;
}
//-----------------------------------------------------------------------------
bool PrepareOptions::inverse() const
{
    return this->mInverse;
}
//-----------------------------------------------------------------------------
void PrepareOptions::setConvType(ConversionType value)
{
    if (value < ConversionTypeMonochrome || value > ConversionTypeColor)
        value = ConversionTypeColor;
    this->mConvType = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setMonoType(MonochromeType value)
{
    if (value < MonochromeTypeEdge || value > MonochromeTypeThresholdDither)
        value = MonochromeTypeDiffuseDither;
    this->mMonoType = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setEdge(int value)
{
    if (value < 0 || value > 255)
        value = 128;
    this->mEdge = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setScanMain(ScanMainDirection value)
{
    if (value < TopToBottom || value > RightToLeft)
        value = TopToBottom;
    this->mScanMain = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setScanSub(ScanSubDirection value)
{
    this->mScanSub = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
void PrepareOptions::setInverse(bool value)
{
    this->mInverse = value;

    emit this->changed();
}
//-----------------------------------------------------------------------------
const QString & PrepareOptions::convTypeName() const
{
    static const QString names[] =
    {
        "Monochrome",
        "Grayscale",
        "Color",
        "???"
    };
    switch (this->convType())
    {
    case ConversionTypeMonochrome:
        return names[0];
    case ConversionTypeGrayscale:
        return names[1];
    case ConversionTypeColor:
        return names[2];
    default:
        return names[3];
    }
}
//-----------------------------------------------------------------------------
const QString & PrepareOptions::monoTypeName() const
{
    static const QString names[] =
    {
        "Edge",
        "Diffuse Dither",
        "Ordered Dither",
        "Threshold Dither",
        "???"
    };
    switch (this->monoType())
    {
    case MonochromeTypeEdge:
        return names[0];
    case MonochromeTypeDiffuseDither:
        return names[1];
    case MonochromeTypeOrderedDither:
        return names[2];
    case MonochromeTypeThresholdDither:
        return names[3];
    default:
        return names[4];
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::modificationsFromScan(
        Rotate *rotate,
        bool *flipHorizontal,
        bool *flipVertical) const
{
    bool forward = (this->mScanSub == Forward);

    switch (this->mScanMain)
    {
    case TopToBottom:
    {
        if (forward)
        {
            *rotate = RotateNone;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        else
        {
            *rotate = RotateNone;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        break;
    }
    case BottomToTop:
    {
        if (forward)
        {
            *rotate = RotateNone;
            *flipHorizontal = false;
            *flipVertical = true;
        }
        else
        {
            *rotate = Rotate180;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        break;
    }
    case LeftToRight:
    {
        if (forward)
        {
            *rotate = Rotate90;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        else
        {
            *rotate = Rotate90;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        break;
    }
    case RightToLeft:
    {
        if (forward)
        {
            *rotate = Rotate270;
            *flipHorizontal = false;
            *flipVertical = false;
        }
        else
        {
            *rotate = Rotate270;
            *flipHorizontal = true;
            *flipVertical = false;
        }
        break;
    }
    }
}
//-----------------------------------------------------------------------------
bool PrepareOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        quint32 uConvType = 0, uMonoType = 0, uEdge = 0;
        quint32 uScanMain = 0, uScanSub = 0, uInverse = 0;

        uConvType = settings->value("convType", int(0)).toUInt(&result);

        if (result)
            uMonoType = settings->value("monoType", int(0)).toUInt(&result);

        if (result)
            uEdge = settings->value("edge", int(0)).toUInt(&result);

        if (result)
            uScanMain = settings->value("scanMain", int(0)).toUInt(&result);

        if (result)
            uScanSub = settings->value("scanSub", int(0)).toUInt(&result);

        if (result)
            uInverse = settings->value("inverse", int(0)).toUInt(&result);

        if (result)
        {
            this->setConvType((ConversionType)uConvType);
            this->setMonoType((MonochromeType)uMonoType);
            this->setEdge((int)uEdge);
            this->setScanMain((ScanMainDirection)uScanMain);
            this->setScanSub((ScanSubDirection)uScanSub);
            this->setInverse((bool)uInverse);
        }
    }
    else if (version == 2)
    {
        settings->beginGroup("prepare");

        result = this->load(settings, 1);

        settings->endGroup();
    }

    return result;
}
//-----------------------------------------------------------------------------
void PrepareOptions::save(QSettings *settings)
{
    settings->beginGroup("prepare");

    settings->setValue("convType", QString("%1").arg((int)this->convType()));
    settings->setValue("monoType", QString("%1").arg((int)this->monoType()));
    settings->setValue("edge",     QString("%1").arg((int)this->edge()));
    settings->setValue("scanMain", QString("%1").arg((int)this->scanMain()));
    settings->setValue("scanSub",  QString("%1").arg((int)this->scanSub()));
    settings->setValue("inverse",  QString("%1").arg((int)this->inverse()));

    settings->endGroup();
}
//-----------------------------------------------------------------------------
