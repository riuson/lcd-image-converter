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

#ifndef CONVIMAGEPIXELS_H
#define CONVIMAGEPIXELS_H

#include <QObject>
#include <QVector>

namespace Parsing
{
namespace Conversion
{

class ConvImagePixels : public QObject
{
  Q_OBJECT
public:
  explicit ConvImagePixels(const QVector<quint32>* data, QObject* parent = 0);
  virtual ~ConvImagePixels();

  Q_PROPERTY(int count READ count)
  Q_PROPERTY(bool processTerminated READ processTerminated WRITE setProcessTerminated)

  Q_INVOKABLE quint32 pixel(int index);
  Q_INVOKABLE void setPixel(int index, quint32 value);

  bool processTerminated() const;
  void setProcessTerminated(bool value);

  void getResults(QVector<quint32>* result) const;

private:
  QVector<quint32> mPixels;
  bool mProcessTerminated;

  int count() const;
};

} // namespace Conversion
} // namespace Parsing

#endif // CONVIMAGEPIXELS_H
