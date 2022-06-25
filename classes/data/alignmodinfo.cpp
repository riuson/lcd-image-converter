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

#include "alignmodinfo.h"
#include "alignmodes.h"

namespace Data
{

void AlignModInfo::Mods::reset()
{
  this->horizontalMode = Data::HorizontalAlignMode::None;
  this->verticalMode = Data::VerticalAlignMode::None;
  this->horizontalOffset = 0;
  this->verticalOffset = 0;
}

AlignModInfo::AlignModInfo()
{
  this->mState.reset();
}

const AlignModInfo::Mods &AlignModInfo::summary() const
{
  return this->mState;
}

void AlignModInfo::modify(
  Data::HorizontalAlignMode horizontalMode,
  int horizontalOffset,
  Data::VerticalAlignMode verticalMode,
  int verticalOffset)
{
  this->mState.horizontalMode = horizontalMode;
  this->mState.verticalMode = verticalMode;
  this->mState.horizontalOffset = horizontalOffset;
  this->mState.verticalOffset = verticalOffset;
}

void AlignModInfo::reset()
{
  this->mState.reset();
}

const QString AlignModInfo::toString() const
{
  return QString("H:%1(%2),V:%3(%4)")
         .arg((int)this->mState.horizontalMode)
         .arg(this->mState.horizontalOffset)
         .arg((int)this->mState.verticalMode)
         .arg(this->mState.verticalOffset);
}

} // namespace Data
