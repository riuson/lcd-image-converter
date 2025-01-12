/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2019 riuson
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

#ifndef CANVASMODINFO_H
#define CANVASMODINFO_H

#include <QString>

namespace Data
{

class CanvasModInfo
{
public:
  struct Mods {
    int left;
    int top;
    int right;
    int bottom;

    void reset();
  };

  CanvasModInfo();

  const Mods summary() const;

  void modify(int left, int top, int right, int bottom);
  void commit();
  void reset();

  const QString toString() const;

private:
  Mods mCommited;
  Mods mModified;
};

} // namespace Data

#endif // CANVASMODINFO_H
