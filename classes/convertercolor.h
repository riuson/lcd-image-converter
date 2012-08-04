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

#ifndef CONVERTERCOLOR_H
#define CONVERTERCOLOR_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"

template <class T> class QQueue;
//-----------------------------------------------------------------------------
class ConverterColor : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterColor(QObject *parent = 0);
    ~ConverterColor();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    enum ColorsOrder
    {
        ColorsOrderRGB = 0,
        ColorsOrderRBG = 1,
        ColorsOrderGRB = 2,
        ColorsOrderGBR = 3,
        ColorsOrderBRG = 4,
        ColorsOrderBGR = 5
    };

    bool swapBytes();
    DataLength length();
    bool mirror();
    bool pack();
    DataAlign align();
    int depthRed();
    int depthGreen();
    int depthBlue();
    ColorsOrder orderRGB();

    void setSwapBytes(bool value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setAlign(DataAlign value);
    void setDepthRed(int value);
    void setDepthGreen(int value);
    void setDepthBlue(int value);
    void setOrderColors(ColorsOrder value);
private:
    bool mSwapBytes;
    DataLength mDataLength;
    bool mMirrorBytes;
    bool mPack;
    DataAlign mDataAlign;
    int mBitsPerPointRed;
    int mBitsPerPointGreen;
    int mBitsPerPointBlue;
    ColorsOrder mOrderColors;

    void makeGradations(QImage &image);
    void queueColors(QQueue<bool> &queue, QRgb value);
    void queueColor(QQueue<bool> &queue, quint8 value, int bitsPerPoint);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERCOLOR_H
