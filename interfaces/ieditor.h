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

#ifndef IEDITOR_H
#define IEDITOR_H

#include <QObject>

namespace Data
{
namespace Containers
{
class DataContainer;
class IDocument;
}
}

namespace AppUI
{
namespace Status
{
class StatusData;
}

class IEditor
{
public:
  enum EditorType {
    EditorImage,
    EditorFont
  };

  virtual Data::Containers::IDocument *document() const = 0;
  virtual QStringList selectedKeys() const = 0;
  virtual AppUI::Status::StatusData *statusData() const = 0;
  virtual EditorType type() const = 0;
};
} // namespace AppUI

Q_DECLARE_INTERFACE (AppUI::IEditor,
                     "riuson.lcd-image-converter/1.0"
                    )

#endif // IEDITOR_H
