/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2017 riuson
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

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>
#include <QIODevice>
#include <QDomDocument>
#include <QMap>

namespace Settings
{

class AppSettings
{
public:
  enum class Section {
    Application,
    Presets
  };

  AppSettings();
  AppSettings(Section section);
  virtual ~AppSettings();

  static void configure(Section section, const QString &filename);
  static void reset();
  QSettings &get();

private:
  static QMap<Section, QString> ConfigFiles;
  static QSettings::Format CustomFormat;
  static QString NameStartChar;
  static QString NameChar;
  static QString NameStartPrefix;
  QSettings *mSettings;

  static bool readXmlFile(QIODevice &device, QSettings::SettingsMap &map);
  static bool writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map);
  static QDomElement getNodeByPath(QDomDocument &doc, const QString &path);
  static void readChilds(QSettings::SettingsMap &map, QStringList &parts, const QDomNodeList &childs);
  static bool readTextNode(QDomNode &node, QString &value);

protected:
  static bool isNameStartCharValid(const QString &value);
  static bool isNameCharValid(const QString &value);
  static bool escape(const QString &source, QString &result);
  static const QString unescape(const QString &value);
};

} // namespace Settings

#endif // APPSETTINGS_H
