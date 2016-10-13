/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#ifndef CONVIMAGE_H
#define CONVIMAGE_H

#include <QObject>
#include <QVector>
#include <QPoint>

class QImage;

class ConvImage : public QObject
{
    Q_OBJECT
public:
    explicit ConvImage(const QImage *image, QObject *parent = 0);
    virtual ~ConvImage();

    Q_PROPERTY(int height READ height)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int bandSize READ bandSize)
    Q_PROPERTY(bool useBands READ useBands)
    Q_PROPERTY(bool scanTerminated READ scanTerminated WRITE setScanTerminated)

    int bandSize() const;
    void setBandSize(int value);

    bool useBands() const;
    void setUseBands(bool value);

    Q_INVOKABLE void addPoint(int x, int y);
    Q_INVOKABLE void clearPoints();

    QPoint pointAt(int index) const;
    int pointsCount() const;

    bool scanTerminated() const;
    void setScanTerminated(bool value);

private:
    const QImage *mImage;
    int mBandSize;
    bool mUseBands;
    QVector<QPoint> mPoints;
    bool mScanTerminated;

    int height() const;
    int width() const;
};

#endif // CONVIMAGE_H
