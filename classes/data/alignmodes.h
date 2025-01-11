/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2022 riuson
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

#ifndef ALIGNMODES_H
#define ALIGNMODES_H

namespace Data
{

enum class HorizontalAlignMode {
  // None.
  None = 0,
  // To left border.
  Left,
  // To center (- 0.5px for odd).
  CenterLeft,
  // To center (+ 0.5px for odd).
  CenterRight,
  // To right border.
  Right,
};

enum class VerticalAlignMode {
  // None.
  None = 0,
  // To top border.
  Top,
  // To center (- 0.5px for odd).
  CenterTop,
  // To center (+ 0.5px for odd).
  CenterBottom,
  // To bottom border.
  Bottom,
};

} // namespace Data

#endif // ALIGNMODES_H
