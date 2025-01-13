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

#ifndef ALIGNMODINFO_H
#define ALIGNMODINFO_H

#include <QString>

namespace Data
{
enum class HorizontalAlignMode;
enum class VerticalAlignMode;
} // namespace Data

namespace Data
{

class AlignModInfo
{
public:
  struct Mods {
    Data::HorizontalAlignMode horizontalMode;
    Data::VerticalAlignMode verticalMode;
    int horizontalOffset;
    int verticalOffset;

    void reset();
  };

  AlignModInfo();

  const Mods& summary() const;

  void modify(HorizontalAlignMode horizontalMode, int horizontalOffset, VerticalAlignMode verticalMode,
              int verticalOffset);
  void reset();

  const QString toString() const;

private:
  Mods mState;
};

} // namespace Data

#endif // ALIGNMODINFO_H
