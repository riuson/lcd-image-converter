/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "convertermono.h"

#include <QSettings>
#include <QImage>
#include <QPainter>

#include "bitmapdata.h"
//-----------------------------------------------------------------------------
ConverterMono::ConverterMono(QObject *parent) :
        QObject(parent)
{
    this->mSwapBytes = false;
    this->mDataLength = Data8;
    this->mMirrorBytes = false;
    this->mDataAlign = IConverter::AlignHigh;
    this->mBlackWhiteLevel = 128;
    this->mDithType = Edge;
}
//-----------------------------------------------------------------------------
ConverterMono::~ConverterMono()
{
}
//-----------------------------------------------------------------------------
void ConverterMono::loadSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("mono");

    bool ok;
    int a;

    this->mSwapBytes = sett.value("swapBytes", QVariant(false)).toBool();

    a = sett.value("dataLength", QVariant(Data8)).toInt(&ok);
    if (ok)
        this->mDataLength = (DataLength)a;

    this->mMirrorBytes = sett.value("mirror", QVariant(false)).toBool();

    a = sett.value("dataAlign", QVariant(AlignHigh)).toInt(&ok);
    if (ok)
        this->mDataAlign = (DataAlign)a;

    a = sett.value("level", QVariant(128)).toInt(&ok);
    if (ok)
        this->mBlackWhiteLevel = a;

    a = sett.value("dithType", QVariant(Edge)).toInt(&ok);
    if (ok)
        this->mDithType = (ConvMonoType)a;

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
void ConverterMono::saveSettings()
{
    QSettings sett;
    sett.beginGroup("converters");
    sett.beginGroup("mono");

    sett.setValue("swapBytes", QVariant(this->mSwapBytes));

    sett.setValue("dataLength", QVariant(this->mDataLength));

    sett.setValue("mirror", QVariant(this->mMirrorBytes));

    sett.setValue("dataAlign", QVariant(this->mDataAlign));

    sett.setValue("level", QVariant(this->mBlackWhiteLevel));

    sett.setValue("dithType", QVariant(this->mDithType));

    sett.endGroup();
    sett.endGroup();
}
//-----------------------------------------------------------------------------
QString ConverterMono::name()
{
    return "mono";
}
//-----------------------------------------------------------------------------
QString ConverterMono::displayName()
{
    return tr("Monochrome");
}
//-----------------------------------------------------------------------------
QImage ConverterMono::preprocessImage(const QImage &source)
{
    QImage result(source);
    switch (this->mDithType)
    {
    case Edge:
        this->makeMonochrome(result);
        break;
    case DiffuseDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::DiffuseDither);
        break;
    case OrderedDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::OrderedDither);
        break;
    case ThresholdDither:
        result = result.convertToFormat(QImage::Format_Mono, Qt::MonoOnly | Qt::ThresholdDither);
        break;
    }

    return result;
}
//-----------------------------------------------------------------------------
void ConverterMono::processImage(const QImage &image, BitmapData *output)
{
    output->clear();
    output->setBlockSize(this->mDataLength);
    output->setWidth(image.width());
    output->setHeight(image.height());

    quint32 data = 0;
    int bitsCounter = 0;
    for (int y = 0; y < image.height(); y++)
    {
        bitsCounter = 0;
        for (int x = 0; x < image.width(); x++)
        {
            QRgb point = image.pixel(x, y);
            // if point is black
            if (qRed(point) == 0)
                data |= (0x01 << (this->mDataLength - bitsCounter - 1));
            bitsCounter++;
            if (bitsCounter >= this->mDataLength)
            {
                output->addBlock(data);
                data = 0;
                bitsCounter = 0;
            }
        }
        if (bitsCounter != 0)
        {
            output->addBlock(data);
            data = 0;
            bitsCounter = 0;
        }
    }
    if (this->mSwapBytes)
        output->swapBytes();
    if (this->mMirrorBytes)
        output->mirrorBytes();
}
//-----------------------------------------------------------------------------
bool ConverterMono::swapBytes()
{
    return this->mSwapBytes;
}
//-----------------------------------------------------------------------------
IConverter::DataLength ConverterMono::length()
{
    return this->mDataLength;
}
//-----------------------------------------------------------------------------
bool ConverterMono::mirror()
{
    return this->mMirrorBytes;
}
//-----------------------------------------------------------------------------
bool ConverterMono::pack()
{
    return true;
}
//-----------------------------------------------------------------------------
IConverter::DataAlign ConverterMono::align()
{
    return this->mDataAlign;
}
//-----------------------------------------------------------------------------
int ConverterMono::level()
{
    return this->mBlackWhiteLevel;
}
//-----------------------------------------------------------------------------
ConverterMono::ConvMonoType ConverterMono::dithType()
{
    return this->mDithType;
}
//-----------------------------------------------------------------------------
void ConverterMono::setSwapBytes(bool value)
{
    this->mSwapBytes = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::setLength(DataLength value)
{
    this->mDataLength = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::setMirror(bool value)
{
    this->mMirrorBytes = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::setPack(bool value)
{
    Q_UNUSED(value);
}
//-----------------------------------------------------------------------------
void ConverterMono::setAlign(DataAlign value)
{
    this->mDataAlign = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::setLevel(int value)
{
    this->mBlackWhiteLevel = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::setDithType(ConvMonoType value)
{
    this->mDithType = value;
}
//-----------------------------------------------------------------------------
void ConverterMono::makeMonochrome(QImage &image)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, false);
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            QRgb value = image.pixel(x, y);
            if (qGray(value) < this->mBlackWhiteLevel)
                painter.setPen(QColor(0, 0, 0));
            else
                painter.setPen(QColor(255, 255, 255));
            painter.drawPoint(x, y);
        }
    }
}//-----------------------------------------------------------------------------
