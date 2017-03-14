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
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringListIterator>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
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
    //qDebug() << path;
    QDomElement nodeValue = AppSettings::getNodeByPath(doc, path);
    QDomText nodeText = doc.createTextNode(value); ///TODO: need user text escape, Qt 5 QString::toHtmlEscaped(), Qt 4 Qt::escape()
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
        qDebug() << "Path: " << path << ", Value: " << value;
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
