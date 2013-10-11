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

#ifndef DEMOGENERATOR_H
#define DEMOGENERATOR_H
//-----------------------------------------------------------------------------
#include <QObject>
#include <QVector>
#include <QPoint>
#include <QTime>
//-----------------------------------------------------------------------------
class Preset;
class QTimer;
class QImage;
class QPixmap;
//-----------------------------------------------------------------------------
class DemoGenerator : public QObject
{
    Q_OBJECT
public:
    explicit DemoGenerator(Preset *preset, QObject *parent = 0);
    virtual ~DemoGenerator();

    void setScript(const QString &value);

    void startAnimation();
    void stopAnimation();

    int animationTime() const;
    void setAnimationTime(int value);

    int animationInterval() const;
    void setAnimationInterval(int value);

private:
    Preset *mPreset;
    QString mScript;
    QTimer *mTimer;
    QVector<QPoint> mPoints;
    int mMax;
    int mIndex;
    QImage *mSourceImage;
    QPixmap *mProcessedImage;
    QTime mLastTick;
    int mAnimationTimeSeconds;

signals:
    void pixmapChanged(const QPixmap *pixmap);
    void errorHandled(const QString &message);

private slots:
    void timeout();
};
//-----------------------------------------------------------------------------
#endif // DEMOGENERATOR_H
