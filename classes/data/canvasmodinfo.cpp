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

#include "canvasmodinfo.h"

namespace Data
{

void CanvasModInfo::Mods::reset()
{
  this->left = this->top = this->right = this->bottom = 0;
}

CanvasModInfo::CanvasModInfo()
{
  this->mCommited.reset();
  this->mModified.reset();
}

const CanvasModInfo::Mods CanvasModInfo::summary() const
{
  CanvasModInfo::Mods result;
  result.top = this->mCommited.top + this->mModified.top;
  result.left = this->mCommited.left + this->mModified.left;
  result.right = this->mCommited.right + this->mModified.right;
  result.bottom = this->mCommited.bottom + this->mModified.bottom;
  return result;
}

void CanvasModInfo::modify(int left, int top, int right, int bottom)
{
  this->mModified.left = left;
  this->mModified.top = top;
  this->mModified.right = right;
  this->mModified.bottom = bottom;
}

void CanvasModInfo::commit()
{
  this->mCommited.left += this->mModified.left;
  this->mCommited.top += this->mModified.top;
  this->mCommited.right += this->mModified.right;
  this->mCommited.bottom += this->mModified.bottom;
  this->mModified.reset();
}

void CanvasModInfo::reset()
{
  this->mCommited.reset();
  this->mModified.reset();
}

const QString CanvasModInfo::toString() const
{
  return QString("L:%1,T:%2,R:%3,B:%4")
         .arg(this->mCommited.left + this->mModified.left)
         .arg(this->mCommited.top + this->mModified.top)
         .arg(this->mCommited.right + this->mModified.right)
         .arg(this->mCommited.bottom + this->mModified.bottom);
}

} // namespace Data
