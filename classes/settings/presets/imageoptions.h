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

#ifndef IMAGEOPTIONS_H
#define IMAGEOPTIONS_H

#include "conversion_options.h"

#include <QObject>

#include "ipresetsoptionspart.h"

namespace Settings
{
namespace Presets
{

class ImageOptions : public QObject, public IPresetOptionsPart
{
  Q_OBJECT
  Q_INTERFACES(Settings::Presets::IPresetOptionsPart)

public:
  explicit ImageOptions(QObject* parent = 0);
  virtual ~ImageOptions() {}

  bool splitToRows() const;
  Parsing::Conversion::Options::BytesOrder bytesOrder() const;
  Parsing::Conversion::Options::DataBlockSize blockSize() const;
  bool blockDefaultOnes() const;
  bool compressionRle() const;
  quint32 compressionRleMinLength() const;
  quint32 blocksPerLine() const;
  QString blockPrefix() const;
  QString blockSuffix() const;
  QString blockDelimiter() const;
  Parsing::Conversion::Options::DataNumeralSystem numeralSystem() const;
  QString previewPrefix() const;
  QString previewSuffix() const;
  QString previewDelimiter() const;
  QString previewLevels() const;

  void setSplitToRows(bool value);
  void setBytesOrder(Parsing::Conversion::Options::BytesOrder value);
  void setBlockSize(Parsing::Conversion::Options::DataBlockSize value);
  void setBlockDefaultOnes(bool value);
  void setCompressionRle(bool value);
  void setCompressionRleMinLength(quint32 value);
  void setBlocksPerLine(quint32 value);
  void setBlockPrefix(const QString& value);
  void setBlockSuffix(const QString& value);
  void setBlockDelimiter(const QString& value);
  void setNumeralSystem(Parsing::Conversion::Options::DataNumeralSystem value);
  void setPreviewPrefix(const QString& value);
  void setPreviewSuffix(const QString& value);
  void setPreviewDelimiter(const QString& value);
  void setPreviewLevels(const QString& value);

  bool load(QSettings* settings) Q_DECL_OVERRIDE;
  bool loadXmlElement(QDomElement element) Q_DECL_OVERRIDE;
  void save(QSettings* settings) Q_DECL_OVERRIDE;
  void saveXmlElement(QDomElement element) Q_DECL_OVERRIDE;

  QString groupName() const Q_DECL_OVERRIDE;

private:
  static const QString GroupName;
  static const QString FieldBytesOrder;
  static const QString FieldBlockSize;
  static const QString FieldBlockDefaultOnes;
  static const QString FieldSplitToRows;
  static const QString FieldCompressionRle;
  static const QString FieldCompressionRleMinLength;
  static const QString FieldBlocksPerLine;
  static const QString FieldBandWidth;
  static const QString FieldBlockPrefix;
  static const QString FieldBlockSuffix;
  static const QString FieldBlockDelimiter;
  static const QString FieldNumeralSystem;
  static const QString FieldPreviewPrefix;
  static const QString FieldPreviewSuffix;
  static const QString FieldPreviewDelimiter;
  static const QString FieldPreviewLevels;

  bool mSplitToRows;
  Parsing::Conversion::Options::BytesOrder mBytesOrder;
  Parsing::Conversion::Options::DataBlockSize mBlockSize;
  bool mBlockDefaultOnes;
  bool mCompressionRle;
  quint32 mCompressionRleMinLength;
  quint32 mBlocksPerLine;
  QString mBlockPrefix;
  QString mBlockSuffix;
  QString mBlockDelimiter;
  Parsing::Conversion::Options::DataNumeralSystem mNumeralSystem;
  QString mPreviewPrefix;
  QString mPreviewSuffix;
  QString mPreviewDelimiter;
  QString mPreviewLevels;

  QString escapeEmpty(const QString& value) const;
  QString unescapeEmpty(const QString& value) const;

signals:
  void changed();

public slots:
};

} // namespace Presets
} // namespace Settings

#endif // IMAGEOPTIONS_H
