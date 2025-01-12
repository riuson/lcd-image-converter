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

#ifndef PARSEDIMAGEDATA_H
#define PARSEDIMAGEDATA_H

#include <QImage>
#include <QObject>

namespace Settings
{
namespace Presets
{
class Preset;
}
} // namespace Settings

namespace Parsing
{
class TagsList;

class ParsedImageData : public QObject
{
  Q_OBJECT
public:
  explicit ParsedImageData(Settings::Presets::Preset* preset, const QImage* image, const TagsList& tags,
                           QObject* parent = 0);
  virtual ~ParsedImageData();

  TagsList* tags() const;
  uint hash() const;

  const QString outputImageDataWithEOL(const TagsList& tags) const;
  const QString outputImagePreviewWithEOL(const TagsList& tags) const;

private:
  TagsList* mTags;
  uint mHash;
  QString mPreparedOutputImageData;
  QString mPreparedOutputImagePreview;
};

} // namespace Parsing

#endif // PARSEDIMAGEDATA_H
