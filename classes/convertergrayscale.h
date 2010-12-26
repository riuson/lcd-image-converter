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

#ifndef CONVERTERGRAYSCALE_H
#define CONVERTERGRAYSCALE_H
//-----------------------------------------------------------------------------
#include <QObject>

#include "iconverter.h"
//-----------------------------------------------------------------------------
class ConverterGrayscale : public QObject, public IConverter
{
    Q_OBJECT
    Q_INTERFACES(IConverter)
public:
    explicit ConverterGrayscale(QObject *parent = 0);
    ~ConverterGrayscale();

    void loadSettings();
    void saveSettings();
    QString name();
    QString displayName();
    QImage preprocessImage(const QImage &source);
    void processImage(const QImage &preprocessedImage, BitmapData *output);

    bool swapBytes();
    DataLength length();
    bool mirror();
    bool pack();
    int depth();

    void setSwapBytes(bool value);
    void setLength(DataLength value);
    void setMirror(bool value);
    void setPack(bool value);
    void setDepth(int value);
private:
    bool mSwapBytes;
    DataLength mDataLength;
    bool mMirrorBytes;
    bool mPack;
    int mBitsPerPoint;

    void makeGrayscale(QImage &image);
};
//-----------------------------------------------------------------------------
#endif // CONVERTERGRAYSCALE_H
