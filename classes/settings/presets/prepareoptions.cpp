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

#include "prepareoptions.h"
//-----------------------------------------------------------------------------
#include <QSettings>
#include <QBuffer>
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
    this->mBandScanning = false;
    this->mBandWidth = 0;
    this->mUseCustomScript = false;
    this->mCustomScript = QString();
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
bool PrepareOptions::bandScanning() const
{
    return this->mBandScanning;
}
//-----------------------------------------------------------------------------
int PrepareOptions::bandWidth() const
{
    if (this->mBandWidth < 1)
        return 1;
    return this->mBandWidth;
}
//-----------------------------------------------------------------------------
bool PrepareOptions::useCustomScript() const
{
    return this->mUseCustomScript;
}
//-----------------------------------------------------------------------------
QString PrepareOptions::customScript() const
{
    return this->mCustomScript;
}
//-----------------------------------------------------------------------------
void PrepareOptions::setConvType(ConversionType value)
{
    if (this->mConvType != value)
    {
        if (value < ConversionTypeMonochrome || value > ConversionTypeColor)
            value = ConversionTypeColor;
        this->mConvType = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setMonoType(MonochromeType value)
{
    if (this->mMonoType != value)
    {
        if (value < MonochromeTypeEdge || value > MonochromeTypeThresholdDither)
            value = MonochromeTypeDiffuseDither;
        this->mMonoType = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setEdge(int value)
{
    if (this->mEdge != value)
    {
        if (value < 0 || value > 255)
            value = 128;
        this->mEdge = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setScanMain(ScanMainDirection value)
{
    if (this->mScanMain != value)
    {
        if (value < TopToBottom || value > RightToLeft)
            value = TopToBottom;
        this->mScanMain = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setScanSub(ScanSubDirection value)
{
    if (this->mScanSub != value)
    {
        this->mScanSub = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setInverse(bool value)
{
    if (this->mInverse != value)
    {
        this->mInverse = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setBandScanning(bool value)
{
    if (this->mBandScanning != value)
    {
        this->mBandScanning = value;
        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setBandWidth(int value)
{
    if (this->mBandWidth != value && value > 0)
    {
        this->mBandWidth = value;

        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setUseCustomScript(bool value)
{
    if (this->mUseCustomScript != value)
    {
        this->mUseCustomScript = value;
        emit this->changed();
    }
}
//-----------------------------------------------------------------------------
void PrepareOptions::setCustomScript(const QString &value)
{
    if (this->mCustomScript != value)
    {
        this->mCustomScript = value;
        emit this->changed();
    }
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
bool PrepareOptions::load(QSettings *settings, int version)
{
    bool result = false;

    if (version == 1)
    {
        quint32 uConvType = 0, uMonoType = 0, uEdge = 0;
        quint32 uScanMain = 0, uScanSub = 0, uInverse = 0;
        quint32 uBandWidth = 1, uBandScanning = 0;
        quint32 uUseCustomScript = 0;
        QString sCustomScript;

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
            uBandScanning = settings->value("bandScanning", false).toBool();

        if (result)
            uBandWidth = settings->value("bandWidth", int(1)).toUInt(&result);

        if (result)
            uUseCustomScript = settings->value("useCustomScript", false).toBool();

        if (result)
        {
            QString str = settings->value("customScript", QString()).toString();
            QByteArray ba = QByteArray::fromBase64(str.toLatin1());
            QBuffer buffer(&ba);
            sCustomScript = QString::fromUtf8(buffer.data());
        }

        if (result)
        {
            this->setConvType((ConversionType)uConvType);
            this->setMonoType((MonochromeType)uMonoType);
            this->setEdge((int)uEdge);
            this->setScanMain((ScanMainDirection)uScanMain);
            this->setScanSub((ScanSubDirection)uScanSub);
            this->setInverse((bool)uInverse);
            this->setBandScanning((bool)uBandScanning);
            this->setBandWidth((int)uBandWidth);
            this->setUseCustomScript((bool)uUseCustomScript);
            this->setCustomScript(sCustomScript);
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
    settings->setValue("bandScanning",    QString("%1").arg((int)this->bandScanning()));
    settings->setValue("bandWidth",       QString("%1").arg((int)this->bandWidth()));
    settings->setValue("useCustomScript", QString("%1").arg((int)this->useCustomScript()));

    {
        QByteArray array = this->mCustomScript.toUtf8();
        array = array.toBase64();
        QString str = QString::fromLatin1(array);
        settings->setValue("customScript", str);
    }

    settings->endGroup();
}
//-----------------------------------------------------------------------------
