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

#ifndef CONVERTERMONO_H
#define CONVERTERMONO_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"
//-----------------------------------------------------------------------------
class ConverterMono : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterMono(QObject *parent = 0);
    ~ConverterMono();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    enum ConvMonoType
    {
        Edge = 0,
        DiffuseDither = 1,
        OrderedDither = 2,
        ThresholdDither = 3
    };

    bool swapBytes();
    DataLength length();
    bool mirror();
    bool pack();
    DataAlign align();
    int level();
    ConvMonoType dithType();

    void setSwapBytes(bool value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setAlign(DataAlign value);
    void setLevel(int value);
    void setDithType(ConvMonoType value);
private:
    bool mSwapBytes;
    DataLength mDataLength;
    bool mMirrorBytes;
    DataAlign mDataAlign;
    int mBlackWhiteLevel;
    ConvMonoType mDithType;
private:
    void makeMonochrome(QImage &image);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERMONO_H
