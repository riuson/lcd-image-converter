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

#ifndef FONTOPTIONS_H
#define FONTOPTIONS_H

#include <QObject>
#include "conversion_options.h"
#include "ipresetsoptionspart.h"

class QStringList;

namespace Settings
{
namespace Presets
{

class FontOptions : public QObject, public IPresetOptionsPart
{
  Q_OBJECT
  Q_INTERFACES(Settings::Presets::IPresetOptionsPart)

public:
  explicit FontOptions(QObject *parent = 0);
  virtual ~FontOptions() {}

  bool bom() const;
  const QString &encoding() const;
  Parsing::Conversion::Options::CharactersSortOrder sortOrder() const;
  bool skipMissingCharacters() const;
  const QString &escapedCharacters() const;
  const QString &escapePrefix() const;
  const QString &escapeSuffix() const;
  bool compactGlyphs() const;

  void setBom(bool value);
  void setEncoding(const QString &value);
  void setSortOrder(Parsing::Conversion::Options::CharactersSortOrder value);
  void setSkipMissingCharacters(bool value);
  void setEscapedCharacters(const QString &value);
  void setEscapePrefix(const QString &value);
  void setEscapeSuffix(const QString &value);
  void setCompactGlyphs(bool value);

  bool load(QSettings *settings) Q_DECL_OVERRIDE;
  bool loadXmlElement(QDomElement element) Q_DECL_OVERRIDE;
  void save(QSettings *settings) Q_DECL_OVERRIDE;
  void saveXmlElement(QDomElement element) Q_DECL_OVERRIDE;

  QString groupName() const Q_DECL_OVERRIDE;

  static const QStringList &encodings();

private:
  static const QString GroupName;
  static const QString FieldBom;
  static const QString FieldSortOrder;
  static const QString FieldCodec;
  static const QString FieldSkipMissingCharacters;
  static const QString FieldEscapedCharacters;
  static const QString FieldEscapePrefix;
  static const QString FieldEscapeSuffix;
  static const QString FieldCompactGlyphs;

  bool mBom;
  QString mEncoding;
  Parsing::Conversion::Options::CharactersSortOrder mSortOrder;
  bool mSkipMissingCharacters;
  QString mEscapedCharacters;
  QString mEscapePrefix;
  QString mEscapeSuffix;
  bool mCompactGlyphs;

signals:
  void changed();

public slots:

};

} // namespace Presets
} // namespace Settings

#endif // FONTOPTIONS_H
