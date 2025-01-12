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

#ifndef PARSER_H
#define PARSER_H

#include <QObject>
#include <QMap>
#include <QRegExp>

#include "conversion_options.h"

namespace Data
{
namespace Containers
{
class IDocument;
class IDataContainer;
}
}

namespace Settings
{
namespace Presets
{
class Preset;
}
}

#include <QImage>

namespace Parsing
{
class TagsList;
class ParsedImageData;

class Parser : public QObject
{
  Q_OBJECT
public:
  enum TemplateType {
    TypeImage,
    TypeFont
  };

  Parser(TemplateType templateType, Settings::Presets::Preset *preset, QObject *parent);
  virtual ~Parser();

  QString name();

  QString convert(Data::Containers::IDocument *document, const QStringList &orderedKeys, QMap<QString, ParsedImageData *> *images, TagsList &tags) const;

private:
  Settings::Presets::Preset *mPreset;
  QString mTemplateFileName;

  QString parse(const QString &templateString, TagsList &tags, Data::Containers::IDocument *doc, const QStringList &orderedKeys, QMap<QString, ParsedImageData *> *images) const;
  QString parseImagesTable(const QString &templateString, TagsList &tags, Data::Containers::IDocument *doc, const QStringList &orderedKeys, QMap<QString, ParsedImageData *> *images) const;
  QString hexCode(const QString &key, const QString &encoding, bool bom) const;
  void addMatrixInfo(TagsList &tags) const;
  void addImagesInfo(TagsList &tags, QMap<QString, ParsedImageData *> *images) const;
  void addCommonInfo(TagsList &tags) const;

  bool findNextTag(int startIndex, int *resultIndex, TagsList);
  void imageDataParticles(const QString &templateString, QString *prefix, QString *suffix) const;
  void imagePreviewParticles(const QString &templateString, QString *prefix, QString *suffix) const;
};
} // namespace Parsing

#endif // PARSER_H
