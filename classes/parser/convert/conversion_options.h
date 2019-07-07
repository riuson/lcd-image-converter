/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2012 riuson
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

#ifndef CONVERSION_OPTIONS_H
#define CONVERSION_OPTIONS_H

#include <QtGlobal>

namespace Parsing
{
namespace Conversion
{
namespace Options
{

enum class ConversionType {
  Monochrome = 0,
  Grayscale  = 1,
  Color      = 2,
  Custom     = 3
};

enum class MonochromeType {
  Edge = 0,
  DiffuseDither = 1,
  OrderedDither = 2,
  ThresholdDither = 3
};

enum class BytesOrder {
  LittleEndian = 0,
  BigEndian = 1
};

enum class DataBlockSize {
  Data8  = 0,
  Data16 = 1,
  Data24 = 2,
  Data32 = 3
};

enum class DataNumeralSystem {
  Binary = 2,
  Octal = 8,
  Decimal = 10,
  Hexadecimal = 16
};

enum class Rotate {
  None = 0,
  Rotate90  =  1,
  Rotate180 =  2,
  Rotate270 =  3
};

enum class ScanMainDirection {
  TopToBottom = 0,
  BottomToTop = 1,
  LeftToRight = 2,
  RightToLeft = 3
};

enum class ScanSubDirection {
  Forward = 0,
  Backward = 1
};

enum class CharactersSortOrder {
  None = 0,
  Ascending = 1,
  Descending = 2
};


} // namespace Options
} // namespace Conversion
} // namespace Parsing

#endif // CONVERSION_OPTIONS_H
