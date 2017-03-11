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

#include "appsettings.h"
#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDataStream>

#include "qt-version-check.h"

QString AppSettings::FileName = "";
QSettings::Format AppSettings::CustomFormat = QSettings::InvalidFormat;

AppSettings::AppSettings()
{
  if (AppSettings::CustomFormat != QSettings::InvalidFormat) {
    this->mSettings = new QSettings(AppSettings::FileName, AppSettings::CustomFormat);
  } else {
    this->mSettings = new QSettings("riuson", "lcd-image-converter");
  }
}

AppSettings::~AppSettings()
{
  delete this->mSettings;
}

void AppSettings::configure(const QString &filename)
{
  AppSettings::FileName = filename;
  QSettings::ReadFunc reader = &AppSettings::readXmlFile;
  QSettings::WriteFunc writer = &AppSettings::writeXmlFile;
  QSettings::Format format = AppSettings::CustomFormat = QSettings::registerFormat("xml", reader, writer);
  QSettings::setDefaultFormat(format);
}

QSettings &AppSettings::get()
{
  return *this->mSettings;
}

bool AppSettings::readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
  QXmlStreamReader xmlReader(&device);

  return false;
}

bool AppSettings::writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
  QXmlStreamWriter xmlWriter(&device);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.setAutoFormattingIndent(2);
  xmlWriter.setCodec("UTF-8");

  xmlWriter.writeStartDocument("1.0", true);
  xmlWriter.writeStartElement("configuration");

  QMapIterator<QString, QVariant> it(map);

  while (it.hasNext()) {
    it.next();
    QString path = it.key();
    QString value = it.value().toString();
    QStringList pathParts = path.split(QChar('/'));
    // write
  }

  xmlWriter.writeEndElement();
  xmlWriter.writeEndDocument();

  return false;
}
