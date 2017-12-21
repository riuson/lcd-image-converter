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

#include "preset.h"

#include <QStringList>
#include <QtXml>
#include <QFile>
#include <QRegExp>
#include <QUuid>
#include <appsettings.h>
#include "prepareoptions.h"
#include "matrixoptions.h"
#include "reorderingoptions.h"
#include "imageoptions.h"
#include "fontoptions.h"
#include "templateoptions.h"

namespace Settings
{
namespace Presets
{

Preset::Preset(QObject *parent) :
  QObject(parent)
{
  this->mBlockChangesSignal = false;

  this->mPrepare    = new PrepareOptions(this);
  this->mMatrix     = new MatrixOptions(this);
  this->mReordering = new ReorderingOptions(this);
  this->mImage      = new ImageOptions(this);
  this->mFont       = new FontOptions(this);
  this->mTemplates  = new TemplateOptions(this);

  this->connect(this->mPrepare,    SIGNAL(changed()), SLOT(partsChanged()));
  this->connect(this->mMatrix,     SIGNAL(changed()), SLOT(partsChanged()));
  this->connect(this->mReordering, SIGNAL(changed()), SLOT(partsChanged()));
  this->connect(this->mImage,      SIGNAL(changed()), SLOT(partsChanged()));
  this->connect(this->mFont,       SIGNAL(changed()), SLOT(partsChanged()));
  this->connect(this->mTemplates,  SIGNAL(changed()), SLOT(partsChanged()));
}

Preset::~Preset()
{
  delete this->mTemplates;
  delete this->mFont;
  delete this->mImage;
  delete this->mReordering;
  delete this->mMatrix;
  delete this->mPrepare;
}

PrepareOptions *Preset::prepare()
{
  return this->mPrepare;
}

MatrixOptions *Preset::matrix()
{
  return this->mMatrix;
}

ReorderingOptions *Preset::reordering()
{
  return this->mReordering;
}

ImageOptions *Preset::image()
{
  return this->mImage;
}

FontOptions *Preset::font()
{
  return this->mFont;
}

TemplateOptions *Preset::templates()
{
  return this->mTemplates;
}

QStringList Preset::presetsList()
{
  AppSettings appsett(AppSettings::Section::Presets);
  QSettings &sett = appsett.get();
  sett.beginGroup("presetsList");
  QStringList groups = sett.childGroups();
  QStringList names;

  foreach (const QString &group, groups) {
    sett.beginGroup(group);
    QString name = sett.value("name").toString();

    if (!name.isEmpty()) {
      names << name;
    }

    sett.endGroup();
  }

  sett.endGroup();

  qSort(names);

  return names;
}

QString Preset::selectedName()
{
  AppSettings appsett(AppSettings::Section::Application);
  QSettings &sett = appsett.get();
  sett.beginGroup("presets");
  QString result = sett.value("selected", QVariant("")).toString();
  sett.endGroup();

  return result;
}

void Preset::setSelectedName(const QString &value)
{
  AppSettings appsett(AppSettings::Section::Application);
  QSettings &sett = appsett.get();
  sett.beginGroup("presets");
  sett.setValue("selected", QVariant(value));
  sett.endGroup();
}

void Preset::remove(const QString &value)
{
  AppSettings appsett(AppSettings::Section::Presets);
  QSettings &sett = appsett.get();
  sett.beginGroup("presetsList");

  QString group = Preset::groupByName(value);
  sett.beginGroup(group);
  sett.remove("");

  sett.endGroup();
}

QString Preset::groupByName(const QString &value)
{
  AppSettings appsett(AppSettings::Section::Presets);
  QSettings &sett = appsett.get();
  sett.beginGroup("presetsList");
  QStringList groups = sett.childGroups();
  QString result;

  foreach (const QString &group, groups) {
    sett.beginGroup(group);
    QString name = sett.value("name").toString();
    sett.endGroup();

    if (name == value) {
      result = group;
      break;
    }
  }

  sett.endGroup();

  return result;
}

QString Preset::name() const
{
  return this->mName;
}

bool Preset::load(const QString &presetName)
{
  bool result = false;

  if (!presetName.isEmpty()) {
    this->mBlockChangesSignal = true;

    AppSettings appsett(AppSettings::Section::Presets);
    QSettings &sett = appsett.get();
    sett.beginGroup("presetsList");

    QString groupName = Preset::groupByName(presetName);

    if (!groupName.isEmpty()) {
      sett.beginGroup(groupName);

      result = this->mPrepare->load(&sett);
      result &= this->mMatrix->load(&sett);
      result &= this->mReordering->load(&sett);
      result &= this->mImage->load(&sett);
      result &= this->mFont->load(&sett);
      result &= this->mTemplates->load(&sett);

      sett.endGroup();

      this->mName = presetName;
    }

    sett.endGroup();

    this->mBlockChangesSignal = false;

    emit this->changed();
  }

  return result;
}

bool Preset::loadXML(const QString &filename)
{
  QDomDocument doc;
  QFile file(filename);

  if (file.open(QIODevice::ReadOnly)) {
    if (doc.setContent(&file)) {
      QDomElement root = doc.documentElement();
      QDomNode nodeName = root.firstChild();

      while (!nodeName.isNull()) {
        QDomElement e = nodeName.toElement();

        if (e.tagName() == "name") {
          this->mName = e.text();
          break;
        }

        nodeName = nodeName.nextSibling();
      }

      if (nodeName.isNull()) {
        return false;
      }

      this->mPrepare->loadXmlElement(root);
      this->mImage->loadXmlElement(root);
      this->mFont->loadXmlElement(root);
      this->mMatrix->loadXmlElement(root);
      this->mReordering->loadXmlElement(root);
      this->mTemplates->loadXmlElement(root);
    }

    file.close();
    return true;
  }

  return false;
}

void Preset::save(const QString &name) const
{
  AppSettings appsett(AppSettings::Section::Presets);
  QSettings &sett = appsett.get();
  sett.beginGroup("presetsList");

  QString group = Preset::groupByName(name);

  if (group.isEmpty()) {
    QUuid id = QUuid::createUuid();
    QRegExp reg("[\\{\\-\\}]");
    group = "p_" + id.toString().remove(reg);
  }

  sett.beginGroup(group);
  sett.remove("");

  sett.setValue("name", name);

  this->mPrepare->save(&sett);
  this->mMatrix->save(&sett);
  this->mReordering->save(&sett);
  this->mImage->save(&sett);
  this->mFont->save(&sett);
  this->mTemplates->save(&sett);

  sett.endGroup();
  sett.endGroup();
}

void Preset::saveXML(const QString &filename) const
{
  QDomDocument doc;
  doc.appendChild(doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));
  QDomElement root = doc.createElement("preset");
  doc.appendChild(root);

  QDomElement nodeName = doc.createElement("name");
  root.appendChild(nodeName);
  nodeName.appendChild(doc.createTextNode(this->mName));

  this->mPrepare->saveXmlElement(root);
  this->mMatrix->saveXmlElement(root);
  this->mReordering->saveXmlElement(root);
  this->mImage->saveXmlElement(root);
  this->mFont->saveXmlElement(root);
  this->mTemplates->saveXmlElement(root);

  QFile outFile(filename);

  if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    qDebug( "Failed to open file for writing." );
    return;
  }

  QTextStream stream(&outFile);
  doc.save(stream, 2);

  outFile.close();
}

void Preset::initMono(MonochromeType type, int edge)
{
  this->mMatrix->operationsRemoveAll();
  this->mReordering->operationsRemoveAll();

  this->mPrepare->setConvType(Parsing::Conversion::Options::ConversionType::Monochrome);
  this->mPrepare->setMonoType(type);
  this->mPrepare->setEdge(edge);
  this->mMatrix->setMaskUsed(0x00000001);
  this->mMatrix->setMaskAnd(0xffffffff);
  this->mMatrix->setMaskOr(0x00000000);
  this->mMatrix->setMaskFill(0x000000ff);
  this->mImage->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data8);

  // alpha bits
  {
    this->mMatrix->operationAdd(0xff000000, 0, false);
  }

  // bits shift
  {
    this->mMatrix->operationAdd(0x00000001, 0, false);
  }

  emit this->changed();
}

void Preset::initGrayscale(int bits)
{
  this->mMatrix->operationsRemoveAll();
  this->mReordering->operationsRemoveAll();

  if (bits > 8) {
    bits = 8;
  }

  if (bits < 1) {
    bits = 1;
  }

  this->mPrepare->setConvType(Parsing::Conversion::Options::ConversionType::Grayscale);
  this->mImage->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data16);

  // mask of used bits before packing
  {
    quint32 mask = 0x000000ff;
    mask = mask << bits;
    mask = mask >> 8;
    mask = mask & 0x000000ff;

    this->mMatrix->setMaskUsed(mask);
  }

  this->mMatrix->setMaskAnd(0xffffffff);
  this->mMatrix->setMaskOr(0x00000000);
  this->mMatrix->setMaskFill(0x0000ffff);

  // alpha bits
  {
    this->mMatrix->operationAdd(0xff000000, 0, false);
  }

  // bits shift
  {
    quint32 mask = 0x0000ff00;
    mask = mask >> bits;
    mask = mask & 0x000000ff;

    quint32 shift = 8 - bits;

    this->mMatrix->operationAdd(mask, shift, false);
  }

  emit this->changed();
}

void Preset::initColor(int alphaBits, int redBits, int greenBits, int blueBits)
{
  this->mMatrix->operationsRemoveAll();
  this->mReordering->operationsRemoveAll();

  if (alphaBits > 8) {
    alphaBits = 8;
  }

  if (alphaBits < 0) {
    alphaBits = 0;
  }

  if (redBits > 8) {
    redBits = 8;
  }

  if (redBits < 1) {
    redBits = 1;
  }

  if (greenBits > 8) {
    greenBits = 8;
  }

  if (greenBits < 1) {
    greenBits = 1;
  }

  if (blueBits > 8) {
    blueBits = 8;
  }

  if (blueBits < 1) {
    blueBits = 1;
  }

  int bits = alphaBits + redBits + greenBits + blueBits;

  this->mPrepare->setConvType(Parsing::Conversion::Options::ConversionType::Color);
  this->mImage->setBlockSize(Parsing::Conversion::Options::DataBlockSize::Data32);

  // mask of used bits before packing
  {
    quint64 mask64 = 0x00000000ffffffff;
    mask64 = mask64 << bits;
    mask64 = mask64 >> 32;
    mask64 = mask64 & 0x00000000ffffffff; // 32 bits
    quint32 mask = (quint32)mask64;
    this->mMatrix->setMaskUsed(mask);
  }

  this->mMatrix->setMaskAnd(0xffffffff);
  this->mMatrix->setMaskOr(0x00000000);
  this->mMatrix->setMaskFill(0xffffffff);

  // alpha bits
  if (alphaBits > 0) {
    quint32 mask = 0x0000ff00;
    mask = mask >> alphaBits;
    mask = mask & 0x000000ff;
    mask = mask << 24;

    quint32 shift = 32 - bits;

    this->mMatrix->operationAdd(mask, shift, false);
  }

  // red bits shift
  {
    quint32 mask = 0x0000ff00;
    mask = mask >> redBits;
    mask = mask & 0x000000ff;
    mask = mask << 16;

    quint32 shift = 24 - redBits - greenBits - blueBits;

    this->mMatrix->operationAdd(mask, shift, false);
  }

  // green bits shift
  {
    quint32 mask = 0x0000ff00;
    mask = mask >> greenBits;
    mask = mask & 0x000000ff;
    mask = mask << 8;

    quint32 shift = 16 - greenBits - blueBits;

    this->mMatrix->operationAdd(mask, shift, false);
  }

  // blue bits shift
  {
    quint32 mask = 0x0000ff00;
    mask = mask >> blueBits;
    mask = mask & 0x000000ff;

    quint32 shift = 8 - blueBits;

    this->mMatrix->operationAdd(mask, shift, false);
  }

  emit this->changed();
}

void Preset::partsChanged()
{
  if (!this->mBlockChangesSignal) {
    emit this->changed();
  }
}

} // namespace Presets
} // namespace Settings
