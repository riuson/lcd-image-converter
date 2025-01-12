/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <QObject>

#include <QImage>
#include <QString>
class StatusData;

namespace Settings
{
namespace Presets
{
class Preset;
}
}

namespace Data
{
namespace Containers
{
class DataContainer;

class IDocument
{
public:
  virtual bool load(const QString &filename) = 0;
  virtual bool save(const QString &filename) = 0;
  virtual bool changed() const = 0;
  virtual QString documentFilename() const = 0;
  virtual QString documentName() const = 0;
  virtual void setDocumentName(const QString &value) = 0;
  virtual QString outputFilename() const = 0;
  virtual void setOutputFilename(const QString &value) = 0;
  virtual DataContainer *dataContainer() const = 0;
  virtual QString convert(Settings::Presets::Preset *preset) = 0;

  virtual void beginChanges() = 0;
  virtual void endChanges(bool suppress) = 0;
  virtual bool canUndo() = 0;
  virtual bool canRedo() = 0;
  virtual void undo() = 0;
  virtual void redo() = 0;
};

} // namespace Containers
} // namespace Data

Q_DECLARE_INTERFACE (Data::Containers::IDocument,
                     "riuson.lcd-image-converter/1.0"
                    )

#endif // IDOCUMENT_H
