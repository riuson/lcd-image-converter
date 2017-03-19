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
#include <QXmlInputSource>

QMap<AppSettings::Section, QString> AppSettings::ConfigFiles;
QSettings::Format AppSettings::CustomFormat = QSettings::InvalidFormat;

// https://www.w3.org/TR/REC-xml/#NT-NameChar
QString AppSettings::NameStartChar = "^(\\:|[A-Z]|_|[a-z]|[\\xC0-\\xD6]|[\\xD8-\\xF6]|[\\xF8-\\x2FF]|[\\x370-\\x37D]|[\\x37F-\\x1FFF]|[\\x200C-\\x200D]|[\\x2070-\\x218F]|[\\x2C00-\\x2FEF]|[\\x3001-\\xD7FF]|[\\xF900-\\xFDCF]|[\\xFDF0-\\xFFFD])";
QString AppSettings::NameChar      = "^(\\:|[A-Z]|_|[a-z]|[\\xC0-\\xD6]|[\\xD8-\\xF6]|[\\xF8-\\x2FF]|[\\x370-\\x37D]|[\\x37F-\\x1FFF]|[\\x200C-\\x200D]|[\\x2070-\\x218F]|[\\x2C00-\\x2FEF]|[\\x3001-\\xD7FF]|[\\xF900-\\xFDCF]|[\\xFDF0-\\xFFFD])"\
                                     "(\\:|[A-Z]|_|[a-z]|[\\xC0-\\xD6]|[\\xD8-\\xF6]|[\\xF8-\\x2FF]|[\\x370-\\x37D]|[\\x37F-\\x1FFF]|[\\x200C-\\x200D]|[\\x2070-\\x218F]|[\\x2C00-\\x2FEF]|[\\x3001-\\xD7FF]|[\\xF900-\\xFDCF]|[\\xFDF0-\\xFFFD]|\\-|\\.|[0-9]|\\xB7|[\\x0300-\\x036F]|[\\x203F-\\x2040])*$";
QString AppSettings::NameStartPrefix = "_escaped_name_start_";

AppSettings::AppSettings()
{
  QString filename = AppSettings::ConfigFiles.value(Section::Application, "");

  if (filename.isEmpty() || AppSettings::CustomFormat == QSettings::InvalidFormat) {
    this->mSettings = new QSettings("riuson", "lcd-image-converter");
  } else {
    this->mSettings = new QSettings(filename, AppSettings::CustomFormat);
  }
}

AppSettings::AppSettings(Section section)
{
  QString filename = AppSettings::ConfigFiles.value(section, "");

  if (filename.isEmpty() || AppSettings::CustomFormat == QSettings::InvalidFormat) {
    this->mSettings = new QSettings("riuson", "lcd-image-converter");
  } else {
    this->mSettings = new QSettings(filename, AppSettings::CustomFormat);
  }
}

AppSettings::~AppSettings()
{
  delete this->mSettings;
}

void AppSettings::configure(Section section, const QString &filename)
{
  AppSettings::ConfigFiles.insert(section, filename);

  if (AppSettings::CustomFormat == QSettings::InvalidFormat) {
    QSettings::ReadFunc reader = &AppSettings::readXmlFile;
    QSettings::WriteFunc writer = &AppSettings::writeXmlFile;
    AppSettings::CustomFormat = QSettings::registerFormat("xml", reader, writer);
  }
}

QSettings &AppSettings::get()
{
  return *this->mSettings;
}

bool AppSettings::readXmlFile(QIODevice &device, QSettings::SettingsMap &map)
{
  QXmlInputSource source(&device);
  QXmlSimpleReader reader;

  QDomDocument doc;
  QString errorMsg;
  int errorColumn, errorLine;

  if (doc.setContent(&source, &reader, &errorMsg, &errorLine, &errorColumn)) {
    QDomElement root = doc.documentElement();

    if (root.tagName() == "configuration") {
      QStringList parts;
      QDomNodeList childs = root.childNodes();
      AppSettings::readChilds(map, parts, childs);
    }

    return true;
  }

  return false;
}

bool AppSettings::writeXmlFile(QIODevice &device, const QSettings::SettingsMap &map)
{
  QDomDocument doc;

  QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
  doc.appendChild(procInstruction);

  QDomComment nodeComment = doc.createComment("lcd-image-converter configuration data");
  doc.appendChild(nodeComment);

  QDomElement nodeRoot = doc.createElement("configuration");
  doc.appendChild(nodeRoot);

  QMapIterator<QString, QVariant> it(map);

  while (it.hasNext()) {
    it.next();
    QString path = it.key();
    QString value = it.value().toString();
    // write
    QDomElement nodeValue = AppSettings::getNodeByPath(doc, path);
    QDomText nodeText = doc.createTextNode(value);
    nodeValue.appendChild(nodeText);
  }

  QTextStream stream(&device);
  doc.save(stream, 4);

  return true;
}

QDomElement AppSettings::getNodeByPath(QDomDocument &doc, const QString &path)
{
  QStringList pathParts = path.split(QChar('/'));

  QDomElement root = doc.documentElement();
  QDomNodeList nodes;
  QDomElement element = root;

  QStringListIterator it(pathParts);

  while (it.hasNext()) {
    QString part = it.next();
    nodes = element.elementsByTagName(part);

    if (nodes.isEmpty()) {
      QDomElement newNode = doc.createElement(part);
      element.appendChild(newNode);
      element = newNode;
    } else if (nodes.count() == 1) {
      element = nodes.at(0).toElement();

      if (element.isNull()) {
      }
    } else {
    }
  }

  return element;
}

void AppSettings::readChilds(QSettings::SettingsMap &map, QStringList &parts, const QDomNodeList &childs)
{
  int length = childs.length();

  for (int i = 0; i < length; i++) {
    QDomNode child = childs.at(i);

    if (child.isElement()) {
      parts.append(child.nodeName());

      QString value;
      bool isText = AppSettings::readTextNode(child, value);

      if (isText) {
        QString path = parts.join("/");
        map.insert(path, value);
      } else {
        AppSettings::readChilds(map, parts, child.childNodes());
      }

      parts.removeLast();
    }
  }
}

bool AppSettings::readTextNode(QDomNode &node, QString &value)
{
  if (node.childNodes().length() == 1) {
    if (node.firstChild().isText()) {
      value = node.firstChild().nodeValue();
      return true;
    }
  }

  return false;
}

bool AppSettings::isNameStartCharValid(const QString &value)
{
  QRegExp regNameStartChar(AppSettings::NameStartChar);
  return (regNameStartChar.indexIn(value) == 0);
}

bool AppSettings::isNameCharValid(const QString &value)
{
  QRegExp regNameChar(AppSettings::NameChar);
  return (regNameChar.indexIn(value) == 0);
}

bool AppSettings::escape(const QString &source, QString &result)
{
  if (AppSettings::isNameCharValid(source)) {
    result = source;
    return true;
  }

  if (AppSettings::isNameStartCharValid(source)) {
    return false;
  }

  QString escaped = AppSettings::NameStartPrefix + source;

  if (AppSettings::isNameCharValid(escaped)) {
    result = escaped;
    return true;
  }

  return false;
}

const QString AppSettings::unescape(const QString &value)
{
  if (value.startsWith(AppSettings::NameStartPrefix)) {
    return value.mid(AppSettings::NameStartPrefix.length());
  }

  return value;
}
