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

#include "demogenerator.h"
//-----------------------------------------------------------------------------
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include "preset.h"
#include "converterhelper.h"
#include "convimage.h"
#include "prepareoptions.h"
//-----------------------------------------------------------------------------
DemoGenerator::DemoGenerator(Preset *preset, QObject *parent) :
    QObject(parent)
{
    this->mPreset = preset;
    this->mTimer = new QTimer(this);
    this->connect(this->mTimer, SIGNAL(timeout()), SLOT(timeout()));
    this->mSourceImage = new QImage(":/demos/scanning_background");
    this->mProcessedImage = new QPixmap();
    this->mLastTick = QTime::currentTime();
    this->mAnimationTimeSeconds = 60;
}
//-----------------------------------------------------------------------------
DemoGenerator::~DemoGenerator()
{
    this->stopAnimation();
    delete this->mTimer;
    delete this->mSourceImage;
}
//-----------------------------------------------------------------------------
void DemoGenerator::setScript(const QString &value)
{
    // stop
    this->stopAnimation();

    // save script
    this->mScript = value;

    // generate points
    ConvImage *convImage = new ConvImage(this->mSourceImage, this);
    convImage->setBandSize(this->mPreset->prepare()->bandWidth());
    convImage->setUseBands(this->mPreset->prepare()->bandScanning());

    QString errorMessage;
    ConverterHelper::collectPoints(convImage, value, &errorMessage);
    if (!errorMessage.isNull())
    {
        emit this->errorHandled(errorMessage);
    }
    else
    {
        // copy points
        this->mMax = convImage->pointsCount();
        this->mIndex = 0;
        this->mPoints.clear();
        if (this->mMax > 0)
        {
            for (int i = 0; i < this->mMax; i++)
            {
                this->mPoints.append(convImage->pointAt(i));
            }

            // start
            this->startAnimation();
        }
    }

    delete convImage;
}
//-----------------------------------------------------------------------------
void DemoGenerator::startAnimation()
{
    this->mTimer->start(50);
}
//-----------------------------------------------------------------------------
void DemoGenerator::stopAnimation()
{
    this->mTimer->stop();
}
//-----------------------------------------------------------------------------
int DemoGenerator::animationTime() const
{
    return this->mAnimationTimeSeconds;
}
//-----------------------------------------------------------------------------
void DemoGenerator::setAnimationTime(int value)
{
    if (value > 1)
        this->mAnimationTimeSeconds = value;
}
//-----------------------------------------------------------------------------
void DemoGenerator::timeout()
{
    QTime current = QTime::currentTime();
    int msecs = this->mLastTick.msecsTo(current); // milliseconds from last frame
    int fps = this->mMax / this->mAnimationTimeSeconds; // frames per second
    int increment = msecs * fps / 1000;

    this->mIndex += increment;

    if ((this->mIndex >= this->mMax) || (this->mIndex < 0))
    {
        this->mIndex = 0;
    }

    QPoint point = this->mPoints.at(this->mIndex);

    QPixmap *pixmap = new QPixmap(QPixmap::fromImage(*this->mSourceImage));
    QPainter painter(pixmap);
    painter.setPen(QColor("silver"));
    if (point.x() >= 0 && point.x() < pixmap->width())
    {
        if (point.y() >= 0 && point.y() < pixmap->height())
        {
            painter.drawLine(point.x(), 0, point.x(), pixmap->height());
            painter.drawLine(0, point.y(), pixmap->width(), point.y());
        }
    }

    delete this->mProcessedImage;
    this->mProcessedImage = pixmap;

    emit this->pixmapChanged(this->mProcessedImage);

    this->mLastTick = current;
}
//-----------------------------------------------------------------------------
