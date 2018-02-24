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

#ifndef PREPAREOPTIONS_H
#define PREPAREOPTIONS_H

#include <QObject>
#include "conversion_options.h"
#include "ipresetsoptionspart.h"

namespace Settings
{
namespace Presets
{

class PrepareOptions : public QObject, public IPresetOptionsPart
{
  Q_OBJECT
  Q_INTERFACES(Settings::Presets::IPresetOptionsPart)

public:
  explicit PrepareOptions(QObject *parent = 0);
  virtual ~PrepareOptions() {}

  Parsing::Conversion::Options::ConversionType convType() const;
  Parsing::Conversion::Options::MonochromeType monoType() const;
  int edge() const;
  Parsing::Conversion::Options::ScanMainDirection scanMain() const;
  Parsing::Conversion::Options::ScanSubDirection scanSub() const;
  bool inverse() const;
  bool bandScanning() const;
  int bandWidth() const;
  bool useCustomScanScript() const;
  QString customScanScript() const;
  QString customPreprocessScript() const;

  void setConvType(Parsing::Conversion::Options::ConversionType value);
  void setMonoType(Parsing::Conversion::Options::MonochromeType value);
  void setEdge(int value);
  void setScanMain(Parsing::Conversion::Options::ScanMainDirection value);
  void setScanSub(Parsing::Conversion::Options::ScanSubDirection value);
  void setInverse(bool value);
  void setBandScanning(bool value);
  void setBandWidth(int value);
  void setUseCustomScanScript(bool value);
  void setCustomScanScript(const QString &value);
  void setCustomPreprocessScript(const QString &value);

  const QString &convTypeName() const;
  const QString &monoTypeName() const;

  bool load(QSettings *settings) Q_DECL_OVERRIDE;
  bool loadXmlElement(QDomElement element) Q_DECL_OVERRIDE;
  void save(QSettings *settings) Q_DECL_OVERRIDE;
  void saveXmlElement(QDomElement element) Q_DECL_OVERRIDE;

  QString groupName() const Q_DECL_OVERRIDE;

private:
  static const QString GroupName;
  static const QString FieldConvType;
  static const QString FieldMonoType;
  static const QString FieldEdge;
  static const QString FieldScanMain;
  static const QString FieldScanSub;
  static const QString FieldInverse;
  static const QString FieldBandScanning;
  static const QString FieldBandWidth;
  static const QString FieldUseCustomScanScript;
  static const QString FieldCustomScanScript;
  static const QString FieldCustomPreprocessScript;

  Parsing::Conversion::Options::ConversionType mConvType;
  Parsing::Conversion::Options::MonochromeType mMonoType;
  int mEdge;
  Parsing::Conversion::Options::ScanMainDirection mScanMain;
  Parsing::Conversion::Options::ScanSubDirection mScanSub;
  bool mInverse;
  bool mBandScanning;
  int mBandWidth;
  bool mUseCustomScanScript;
  QString mCustomScanScript;
  QString mCustomPreprocessScript;

signals:
  void changed();

public slots:

};

} // namespace Presets
} // namespace Settings

#endif // PREPAREOPTIONS_H
