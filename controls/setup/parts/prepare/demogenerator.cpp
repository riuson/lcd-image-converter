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
#include "preset.h"
//-----------------------------------------------------------------------------
DemoGenerator::DemoGenerator(Preset *preset, QObject *parent) :
    QObject(parent)
{
    this->mPreset = preset;
    this->mTimer = new QTimer(this);
}
//-----------------------------------------------------------------------------
DemoGenerator::~DemoGenerator()
{
    this->stopAnimation();
    delete this->mTimer;
}
//-----------------------------------------------------------------------------
void DemoGenerator::setScript(const QString &value)
{
    // stop
    this->stopAnimation();

    // save script
    this->mScript = value;

    // generate points

    // start
    this->startAnimation();
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
