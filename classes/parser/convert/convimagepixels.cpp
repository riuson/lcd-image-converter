/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2017 riuson
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

#include "convimagepixels.h"

#include <QImage>

ConvImagePixels::ConvImagePixels(const QVector<quint32> *data, QObject *parent) :
  QObject(parent)
{
  // void QVector::append(const QVector<T> &value) implemented in Qt >= 5.5
  int dataCount = data->size();

  for (int i = 0; i < dataCount; i++) {
    this->mPixels.append(data->at(i));
  }

  this->mProcessTerminated = false;
}

ConvImagePixels::~ConvImagePixels()
{
}

quint32 ConvImagePixels::pixel(int index)
{
  return this->mPixels.at(index);
}

void ConvImagePixels::setPixel(int index, quint32 value)
{
  this->mPixels.replace(index, value);
}

bool ConvImagePixels::processTerminated() const
{
  return this->mProcessTerminated;
}

void ConvImagePixels::setProcessTerminated(bool value)
{
  this->mProcessTerminated = value;
}

void ConvImagePixels::getResults(QVector<quint32> *result) const
{
  result->clear();
  // void QVector::append(const QVector<T> &value) implemented in Qt >= 5.5
  int dataCount = this->mPixels.size();

  for (int i = 0; i < dataCount; i++) {
    result->append(this->mPixels.at(i));
  }
}

int ConvImagePixels::count() const
{
  return this->mPixels.size();
}

