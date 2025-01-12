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

#include "convimagescan.h"

#include <QImage>

namespace Parsing
{
namespace Conversion
{

ConvImageScan::ConvImageScan(const QImage* image, QObject* parent) : QObject(parent)
{
  this->mImage = image;
  this->mBandSize = 1;
  this->mUseBands = false;
  this->mScanTerminated = false;
}

ConvImageScan::~ConvImageScan() {}

int ConvImageScan::bandSize() const { return this->mBandSize; }

void ConvImageScan::setBandSize(int value) { this->mBandSize = value; }

bool ConvImageScan::useBands() const { return this->mUseBands; }

void ConvImageScan::setUseBands(bool value) { this->mUseBands = value; }

void ConvImageScan::addPoint(int x, int y) { this->mPoints.append(QPoint(x, y)); }

void ConvImageScan::clearPoints() { this->mPoints.clear(); }

QPoint ConvImageScan::pointAt(int index) const
{
  if (index < this->mPoints.size()) {
    return this->mPoints.at(index);
  }

  return QPoint();
}

int ConvImageScan::pointsCount() const { return this->mPoints.size(); }

bool ConvImageScan::scanTerminated() const { return this->mScanTerminated; }

void ConvImageScan::setScanTerminated(bool value) { this->mScanTerminated = value; }

int ConvImageScan::height() const { return this->mImage->height(); }

int ConvImageScan::width() const { return this->mImage->width(); }

} // namespace Conversion
} // namespace Parsing
