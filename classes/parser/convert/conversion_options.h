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

namespace ConversionOptions
{


enum ConversionType {
  ConversionTypeMonochrome = 0,
  ConversionTypeGrayscale  = 1,
  ConversionTypeColor      = 2,
  ConversionTypeCustom     = 3
};

enum MonochromeType {
  MonochromeTypeEdge = 0,
  MonochromeTypeDiffuseDither = 1,
  MonochromeTypeOrderedDither = 2,
  MonochromeTypeThresholdDither = 3
};

enum ColorsOrder {
  ColorsOrderRGB = 0,
  ColorsOrderRBG = 1,
  ColorsOrderGRB = 2,
  ColorsOrderGBR = 3,
  ColorsOrderBRG = 4,
  ColorsOrderBGR = 5
};

enum BytesOrder {
  BytesOrderLittleEndian = 0,
  BytesOrderBigEndian = 1
};

enum DataBlockSize {
  Data8  = 0,
  Data16 = 1,
  Data24 = 2,
  Data32 = 3
};

enum Rotate {
  RotateNone = 0,
  Rotate90  =  1,
  Rotate180 =  2,
  Rotate270 =  3
};

enum ScanMainDirection {
  TopToBottom = 0,
  BottomToTop = 1,
  LeftToRight = 2,
  RightToLeft = 3
};

enum ScanSubDirection {
  Forward = 0,
  Backward = 1
};

enum CharactersSortOrder {
  CharactersSortNone = 0,
  CharactersSortAscending = 1,
  CharactersSortDescending = 2
};


} // end of namespace

#endif // CONVERSION_OPTIONS_H
