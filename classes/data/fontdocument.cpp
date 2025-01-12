/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
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

#include "fontdocument.h"

#include <QBuffer>
#include <QDomDocument>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QImage>
#include <QModelIndex>
#include <QModelIndexList>
#include <QPainter>
#include <QTextCodec>
#include <QTextStream>
#include <QWidget>

#include "bitmaphelper.h"
#include "datacontainer.h"
#include "dialogfontchanged.h"
#include "fonteditoroptions.h"
#include "fonthelper.h"
#include "fontoptions.h"
#include "fontparameters.h"
#include "fontsizeunits.h"
#include "parsedimagedata.h"
#include "parser.h"
#include "preset.h"
#include "tagslist.h"

namespace Data
{
namespace Containers
{

FontDocument::FontDocument(QObject* parent) : QObject(parent)
{
  this->mContainer = new DataContainer(this);
  this->connect(this->mContainer, SIGNAL(dataChanged(bool)), SLOT(mon_container_dataChanged(bool)));

  this->mNestedChangesCounter = 0;

  this->beginChanges();

  this->setDocumentName(QString("Font"));
  this->setDocumentFilename("");
  this->setOutputFilename("");
  this->setAntialiasing(false);
  this->setMonospaced(false);

  this->endChanges(true);
  this->mContainer->historyInit();
}

FontDocument::~FontDocument() {}

bool FontDocument::load(const QString& fileName)
{
  bool result = false;

  QFile file(fileName);

  if (file.open(QIODevice::ReadOnly)) {
    this->beginChanges();

    QDomDocument doc;
    QString errorMsg;
    QString converted;
    int errorColumn, errorLine;

    this->mContainer->blockSignals(true);

    if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
      QDomElement root = doc.documentElement();

      if (root.tagName() == "data") {
        result = true;
        this->setDocumentName(root.attribute("name", fileName));

        QString fontFamily, style;
        int size = 0;
        int ascent = 0, descent = 0;
        bool monospaced = false, antialiasing = false;
        QColor foreground = Settings::FontEditorOptions::foreColor();
        QColor background = Settings::FontEditorOptions::backColor();
        int multiplicityWidth = 1, multiplicityHeight = 1;
        FontSizeUnits sizeUnits = FontSizeUnits::Pixels;

        QDomNode n = root.firstChild();

        while (!n.isNull()) {
          QDomElement e = n.toElement();

          if (!e.isNull()) {
            if (e.tagName() == "family") {
              fontFamily = e.text();
            } else if (e.tagName() == "style") {
              style = e.text();
            } else if (e.tagName() == "size") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                size = a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "ascent") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                ascent = a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "descent") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                descent = a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "widthType") {
              monospaced = (e.text() == "monospaced");
            } else if (e.tagName() == "antialiasing") {
              antialiasing = (e.text() == "true");
            } else if (e.tagName() == "foreground") {
              bool ok;
              QString str = e.text();
              quint32 a = str.toUInt(&ok, 16);

              if (ok) {
                foreground = Parsing::Conversion::BitmapHelper::fromRgba(QRgb(a));
              } else {
                result = false;
              }
            } else if (e.tagName() == "background") {
              bool ok;
              QString str = e.text();
              quint32 a = str.toUInt(&ok, 16);

              if (ok) {
                background = Parsing::Conversion::BitmapHelper::fromRgba(QRgb(a));
              } else {
                result = false;
              }
            } else if (e.tagName() == "multiplicityWidth") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                multiplicityWidth = a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "multiplicityHeight") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                multiplicityHeight = a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "sizeUnits") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok && ((a == (int)FontSizeUnits::Pixels) || (a == (int)FontSizeUnits::Points))) {
                sizeUnits = (FontSizeUnits)a;
              } else {
                result = false;
              }
            } else if (e.tagName() == "converted") {
              converted = e.text();
            }
          }

          n = n.nextSibling();
        }

        QDomNodeList nodesChar = root.elementsByTagName("char");

        for (int i = 0; i < nodesChar.count(); i++) {
          QDomElement e = nodesChar.at(i).toElement();
          bool ok;
          int code = e.attribute("code", "none").toInt(&ok, 16);

          if (ok) {
            QDomElement nodePicture = e.firstChildElement("picture");

            if (nodePicture.isElement()) {
              QString str = nodePicture.text();
              QByteArray ba = QByteArray::fromBase64(str.toLatin1());
              QBuffer buffer(&ba);
              QImage image;
              image.load(&buffer, "PNG");

              QString key = QString(QChar(code));
              this->mContainer->setImage(key, &image);
            }
          } else {
            result = false;
          }
        }

        QFontDatabase fonts;
        QFont font = fonts.font(fontFamily, style, size);

        switch (sizeUnits) {
          default:
          case Data::FontSizeUnits::Pixels: {
            font.setPixelSize(size);
            break;
          }

          case Data::FontSizeUnits::Points: {
            font.setPointSize(size);
            break;
          }
        }

        this->setSizeUnits(sizeUnits);
        this->setUsedFont(font);

        this->setUsedStyle(style);
        this->setMonospaced(monospaced);
        this->setAntialiasing(antialiasing);
        this->setForeground(foreground);
        this->setBackground(background);
        this->setAscent(ascent);
        this->setDescent(descent);
        this->setMultiplicityWidth(multiplicityWidth);
        this->setMultiplicityHeight(multiplicityHeight);
      }
    }

    file.close();

    this->mContainer->blockSignals(false);

    this->setDocumentFilename(fileName);
    this->setOutputFilename(converted);

    this->endChanges(true);

    this->mContainer->historyInit();
  }

  return result;
}

bool FontDocument::save(const QString& fileName)
{
  bool result = false;

  QDomDocument doc;
  QDomProcessingInstruction procInstruction =
      doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
  doc.appendChild(procInstruction);

  QDomElement nodeRoot = doc.createElement("data");
  doc.appendChild(nodeRoot);

  QString chars;
  FontParameters parameters;
  this->fontCharacters(&chars, &parameters);

  nodeRoot.setAttribute("type", "font");
  nodeRoot.setAttribute("name", this->documentName());

  // font family
  QDomElement nodeFamily = doc.createElement("family");
  nodeRoot.appendChild(nodeFamily);
  nodeFamily.appendChild(doc.createTextNode(parameters.family));

  // size
  QDomElement nodeSize = doc.createElement("size");
  nodeRoot.appendChild(nodeSize);
  nodeSize.appendChild(doc.createTextNode(QString("%1").arg(parameters.size)));

  // sizeUnits
  QDomElement nodeSizeUnits = doc.createElement("sizeUnits");
  nodeRoot.appendChild(nodeSizeUnits);
  nodeSizeUnits.appendChild(doc.createTextNode(QString("%1").arg((int)parameters.sizeUnits)));

  // ascent
  QDomElement nodeAscent = doc.createElement("ascent");
  nodeRoot.appendChild(nodeAscent);
  nodeAscent.appendChild(doc.createTextNode(QString("%1").arg(parameters.ascent)));

  // descent
  QDomElement nodeDescent = doc.createElement("descent");
  nodeRoot.appendChild(nodeDescent);
  nodeDescent.appendChild(doc.createTextNode(QString("%1").arg(parameters.descent)));

  // style
  QDomElement nodeStyle = doc.createElement("style");
  nodeRoot.appendChild(nodeStyle);
  nodeStyle.appendChild(doc.createTextNode(parameters.style));

  // monospaced or proportional
  QDomElement nodeWidthType = doc.createElement("widthType");
  nodeRoot.appendChild(nodeWidthType);

  if (parameters.monospaced) {
    nodeWidthType.appendChild(doc.createTextNode("monospaced"));
  } else {
    nodeWidthType.appendChild(doc.createTextNode("proportional"));
  }

  // antialiasing
  QDomElement nodeAntialiasing = doc.createElement("antialiasing");
  nodeRoot.appendChild(nodeAntialiasing);

  if (parameters.antiAliasing) {
    nodeAntialiasing.appendChild(doc.createTextNode("true"));
  } else {
    nodeAntialiasing.appendChild(doc.createTextNode("false"));
  }

  // foreground
  QDomElement nodeForeground = doc.createElement("foreground");
  nodeRoot.appendChild(nodeForeground);
  nodeForeground.appendChild(
      doc.createTextNode(QString("%1").arg((quint32)parameters.foreground.rgba(), 8, 16, QChar('0'))));

  // background
  QDomElement nodeBackground = doc.createElement("background");
  nodeRoot.appendChild(nodeBackground);
  nodeBackground.appendChild(
      doc.createTextNode(QString("%1").arg((quint32)parameters.background.rgba(), 8, 16, QChar('0'))));

  // multiplicity width
  QDomElement nodeMultiplicityWidth = doc.createElement("multiplicityWidth");
  nodeRoot.appendChild(nodeMultiplicityWidth);
  nodeMultiplicityWidth.appendChild(doc.createTextNode(QString("%1").arg(parameters.multiplicityWidth)));

  // multiplicity height
  QDomElement nodeMultiplicityHeight = doc.createElement("multiplicityHeight");
  nodeRoot.appendChild(nodeMultiplicityHeight);
  nodeMultiplicityHeight.appendChild(doc.createTextNode(QString("%1").arg(parameters.multiplicityHeight)));

  // string
  QDomElement nodeString = doc.createElement("string");
  nodeRoot.appendChild(nodeString);
  nodeString.appendChild(doc.createTextNode(Parsing::Conversion::FontHelper::escapeControlChars(chars)));

  // converted file name
  QDomElement nodeConverted = doc.createElement("converted");
  nodeRoot.appendChild(nodeConverted);
  nodeConverted.appendChild(doc.createTextNode(this->outputFilename()));

  // chars list
  QDomElement nodeChars = doc.createElement("chars");
  nodeRoot.appendChild(nodeChars);

  QListIterator<QString> it(this->mContainer->keys());
  it.toFront();

  while (it.hasNext()) {
    QString key = it.next();
    // char
    QDomElement nodeChar = doc.createElement("char");
    nodeChars.appendChild(nodeChar);
    nodeChar.setAttribute("character", Parsing::Conversion::FontHelper::escapeControlChars(key));
    nodeChar.setAttribute("code", QString("%1").arg(key.at(0).unicode(), 4, 16, QChar('0')));

    QDomElement nodePicture = doc.createElement("picture");
    nodeChar.appendChild(nodePicture);

    nodePicture.setAttribute("format", "png");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    this->mContainer->image(key)->save(&buffer, "PNG");
    QString data = ba.toBase64();

    QDomText nodeData = doc.createTextNode(data);
    nodePicture.appendChild(nodeData);
  }

  QFile file(fileName);

  if (file.open(QIODevice::WriteOnly)) {
    QTextStream stream(&file);
    doc.save(stream, 4);

    this->beginChanges();

    this->setDocumentFilename(fileName);
    file.close();
    result = true;

    this->endChanges(true);
  }

  return result;
}

bool FontDocument::changed() const
{
  bool result = this->mContainer->changed();
  return result;
}

QString FontDocument::documentFilename() const
{
  QVariant result = this->mContainer->commonInfo("filename");
  return result.toString();
}

QString FontDocument::documentName() const
{
  QVariant result = this->mContainer->commonInfo("document name");
  return result.toString();
}

void FontDocument::setDocumentName(const QString& value) { this->mContainer->setCommonInfo("document name", value); }

QString FontDocument::outputFilename() const
{
  QVariant result = this->mContainer->commonInfo("converted filename");
  return result.toString();
}

void FontDocument::setOutputFilename(const QString& value)
{
  this->mContainer->setCommonInfo("converted filename", QVariant(value));
}

DataContainer* FontDocument::dataContainer() const { return this->mContainer; }

QString FontDocument::convert(Settings::Presets::Preset* preset)
{
  Parsing::TagsList tags;

  if (!this->documentFilename().isEmpty()) {
    tags.setTagValue(Parsing::TagsList::Tag::DocumentFilename, this->documentFilename());
  } else {
    tags.setTagValue(Parsing::TagsList::Tag::DocumentFilename, "unsaved");
  }

  tags.setTagValue(Parsing::TagsList::Tag::DocumentName, this->documentName());
  tags.setTagValue(Parsing::TagsList::Tag::DocumentNameWithoutSpaces,
                   this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));
  tags.setTagValue(Parsing::TagsList::Tag::DocumentNameWithoutSpacesUpperCase,
                   this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)).toUpper());
  tags.setTagValue(Parsing::TagsList::Tag::DocumentNameWithoutSpacesLowerCase,
                   this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)).toLower());

  QString chars;
  FontParameters parameters;
  this->fontCharacters(&chars, &parameters);

  tags.setTagValue(Parsing::TagsList::Tag::DocumentDataType, "font");
  tags.setTagValue(Parsing::TagsList::Tag::FontFamily, parameters.family);
  tags.setTagValue(Parsing::TagsList::Tag::FontSize, QString("%1").arg(parameters.size));
  tags.setTagValue(Parsing::TagsList::Tag::FontSizeUnits,
                   parameters.sizeUnits == Data::FontSizeUnits::Points ? "pt" : "px");
  tags.setTagValue(Parsing::TagsList::Tag::FontStyle, parameters.style);
  tags.setTagValue(Parsing::TagsList::Tag::FontString, Parsing::Conversion::FontHelper::escapeControlChars(chars));
  tags.setTagValue(Parsing::TagsList::Tag::FontAntiAliasing, parameters.antiAliasing ? "yes" : "no");
  tags.setTagValue(Parsing::TagsList::Tag::FontWidthType, parameters.monospaced ? "monospaced" : "proportional");
  tags.setTagValue(Parsing::TagsList::Tag::FontAscent, QString("%1").arg(parameters.ascent));
  tags.setTagValue(Parsing::TagsList::Tag::FontDescent, QString("%1").arg(parameters.descent));

  const QStringList orderedKeys = this->sortKeysWithEncoding(this->dataContainer()->keys(), preset);

  QMap<QString, Parsing::ParsedImageData*> images;
  this->prepareImages(preset, orderedKeys, &images, tags);
  Parsing::Parser parser(Parsing::Parser::TypeFont, preset, this);
  QString result = parser.convert(this, orderedKeys, &images, tags);

  return result;
}

void FontDocument::beginChanges()
{
  if (!this->mContainer->historyInitialized()) {
    this->mContainer->historyInit();
  }

  this->mNestedChangesCounter++;
}

void FontDocument::endChanges(bool suppress)
{
  if (this->mNestedChangesCounter > 0) {
    if (--this->mNestedChangesCounter == 0) {
      bool changed = this->mContainer->changed();

      if (suppress) {
        this->mContainer->setChanged(false);
        changed = false;
      }

      if (changed) {
        this->mContainer->stateSave();
      }

      emit this->documentChanged();
    }
  }
}

bool FontDocument::canUndo() { return this->mContainer->canUndo(); }

bool FontDocument::canRedo() { return this->mContainer->canRedo(); }

void FontDocument::undo()
{
  this->mContainer->stateUndo();

  emit this->documentChanged();
}

void FontDocument::redo()
{
  this->mContainer->stateRedo();

  emit this->documentChanged();
}

void FontDocument::fontCharacters(QString* chars, Data::Containers::FontParameters* parameters)
{
  QStringList charList(this->mContainer->keys());
  *chars = charList.join("");
  parameters->family = this->usedFont().family();
  parameters->sizeUnits = this->sizeUnits();

  switch (this->sizeUnits()) {
    default:
    case FontSizeUnits::Pixels: {
      parameters->size = this->usedFont().pixelSize();
      break;
    }

    case FontSizeUnits::Points: {
      parameters->size = this->usedFont().pointSize();
      break;
    }
  }

  parameters->style = this->usedStyle();
  parameters->monospaced = this->monospaced();
  parameters->antiAliasing = this->antialiasing();
  parameters->foreground = this->foreground();
  parameters->background = this->background();
  parameters->ascent = this->ascent();
  parameters->descent = this->descent();
  parameters->multiplicityHeight = this->multiplicityHeight();
  parameters->multiplicityWidth = this->multiplicityWidth();
}

void FontDocument::setFontCharacters(const QString& chars, const Data::Containers::FontParameters& parameters)
{
  QFontDatabase fonts;

  bool regenerateAll = false;

  this->mContainer->blockSignals(true);

  if (this->mContainer->count() > 1) {
    if (this->usedFont().family() != parameters.family || this->usedStyle() != parameters.style ||
        this->sizeUnits() != parameters.sizeUnits ||
        ((parameters.sizeUnits == FontSizeUnits::Pixels) && (this->usedFont().pixelSize() != parameters.size)) ||
        ((parameters.sizeUnits == FontSizeUnits::Points) && (this->usedFont().pointSize() != parameters.size)) ||
        this->monospaced() != parameters.monospaced || this->antialiasing() != parameters.antiAliasing ||
        this->foreground() != parameters.foreground || this->background() != parameters.background ||
        this->ascent() != parameters.ascent || this->descent() != parameters.descent ||
        this->multiplicityHeight() != parameters.multiplicityHeight ||
        this->multiplicityWidth() != parameters.multiplicityWidth) {
      AppUI::Fonts::DialogFontChanged dialog(qobject_cast<QWidget*>(this->parent()));

      if (dialog.exec() == QDialog::Accepted) {
        regenerateAll = dialog.regenerateAll();
      } else {
        return;
      }
    }
  } else { // for newly created fonts
    regenerateAll = true;
  }

  this->beginChanges();

  // create font with specified parameters
  QFont fontNew = fonts.font(parameters.family, parameters.style, parameters.size);

  switch (parameters.sizeUnits) {
    case FontSizeUnits::Points: {
      fontNew.setPointSize(parameters.size);
      break;
    }

    default:
    case FontSizeUnits::Pixels: {
      fontNew.setPixelSize(parameters.size);
      break;
    }
  }

  if (parameters.antiAliasing) {
    fontNew.setStyleStrategy(QFont::PreferAntialias);
  } else {
    fontNew.setStyleStrategy(QFont::NoAntialias);
  }

  // remove old characters
  if (regenerateAll) {
    this->mContainer->clear();

    // save new font
    this->setUsedFont(fontNew);
    this->setUsedStyle(parameters.style);
    this->setMonospaced(parameters.monospaced);
    this->setAntialiasing(parameters.antiAliasing);
    this->setAscent(parameters.ascent);
    this->setDescent(parameters.descent);
    this->setForeground(parameters.foreground);
    this->setBackground(parameters.background);
    this->setMultiplicityHeight(parameters.multiplicityHeight);
    this->setMultiplicityWidth(parameters.multiplicityWidth);
    this->setSizeUnits(parameters.sizeUnits);
  } else {
    // remove characters, which not present in new characters list
    QStringList keys = this->mContainer->keys();
    QListIterator<QString> it(keys);
    it.toFront();

    while (it.hasNext()) {
      QString a = it.next();

      if (!chars.contains(a)) {
        this->mContainer->removeImage(a);
      }
    }
  }

  // find max size
  int width = 0, height = 0;
  QFontMetrics metrics(fontNew);

  if (parameters.monospaced) {
    for (int i = 0; i < chars.count(); i++) {
      width = qMax(width, metrics.horizontalAdvance(chars.at(i)));
    }

    height = metrics.height();
  }

  // generate new characters
  QStringList keys = this->mContainer->keys();
  QStringList userOrdered;

  for (int i = 0; i < chars.count(); i++) {
    QString key = QString(chars.at(i));
    userOrdered.append(key);

    // if character not exists, create it
    if (!keys.contains(key)) {
      keys.append(key);
      QImage image = Parsing::Conversion::FontHelper::drawCharacter(
          chars.at(i), fontNew, parameters.foreground, parameters.background, width, height, parameters.antiAliasing,
          parameters.multiplicityHeight, parameters.multiplicityWidth);
      this->mContainer->setImage(key, new QImage(image));
    }
  }

  this->mContainer->reorderTo(&userOrdered);

  this->mContainer->blockSignals(false);

  this->endChanges(false);
}

void FontDocument::setDocumentFilename(const QString& value)
{
  if (this->documentFilename() != value) {
    this->mContainer->setCommonInfo("filename", QVariant(value));
  }
}

QFont FontDocument::usedFont() const
{
  QVariant var = this->mContainer->commonInfo("used font");

  if (!var.isNull()) {
    QFont result = var.value<QFont>();
    return result;
  }

  return QFont();
}

void FontDocument::setUsedFont(const QFont& value) { this->mContainer->setCommonInfo("used font", value); }

QString FontDocument::usedStyle() const { return this->mContainer->commonInfo("style").toString(); }

void FontDocument::setUsedStyle(const QString& value) { this->mContainer->setCommonInfo("style", value); }

bool FontDocument::monospaced() const { return this->mContainer->commonInfo("monospaced").toBool(); }

void FontDocument::setMonospaced(const bool value) { this->mContainer->setCommonInfo("monospaced", value); }

bool FontDocument::antialiasing() const { return this->mContainer->commonInfo("antialiasing").toBool(); }

void FontDocument::setAntialiasing(const bool value) { this->mContainer->setCommonInfo("antialiasing", value); }

QColor FontDocument::foreground() const
{
  bool ok;
  quint32 rgbValue = this->mContainer->commonInfo("foreground").toUInt(&ok);

  if (ok) {
    return Parsing::Conversion::BitmapHelper::fromRgba(QRgb(rgbValue));
  }

  return Settings::FontEditorOptions::foreColor();
}

void FontDocument::setForeground(const QColor value)
{
  this->mContainer->setCommonInfo("foreground", (quint32)value.rgba());
}

QColor FontDocument::background() const
{
  bool ok;
  quint32 rgbValue = this->mContainer->commonInfo("background").toUInt(&ok);

  if (ok) {
    return Parsing::Conversion::BitmapHelper::fromRgba(QRgb(rgbValue));
  }

  return Settings::FontEditorOptions::backColor();
}

void FontDocument::setBackground(const QColor value)
{
  this->mContainer->setCommonInfo("background", (quint32)value.rgba());
}

int FontDocument::ascent() const { return this->mContainer->commonInfo("ascent").toInt(); }

void FontDocument::setAscent(int value) { this->mContainer->setCommonInfo("ascent", value); }

int FontDocument::descent() const { return this->mContainer->commonInfo("descent").toInt(); }

void FontDocument::setDescent(int value) { this->mContainer->setCommonInfo("descent", value); }

int FontDocument::multiplicityWidth() const { return this->mContainer->commonInfo("multiplicityWidth").toInt(); }

void FontDocument::setMultiplicityWidth(const int value)
{
  if (value > 0) {
    this->mContainer->setCommonInfo("multiplicityWidth", value);
  }
}

int FontDocument::multiplicityHeight() const { return this->mContainer->commonInfo("multiplicityHeight").toInt(); }

void FontDocument::setMultiplicityHeight(const int value)
{
  if (value > 0) {
    this->mContainer->setCommonInfo("multiplicityHeight", value);
  }
}

FontSizeUnits FontDocument::sizeUnits() const
{
  return (FontSizeUnits)this->mContainer->commonInfo("sizeUnits").toInt();
}

void FontDocument::setSizeUnits(const FontSizeUnits sizeUnits)
{
  this->mContainer->setCommonInfo("sizeUnits", (int)sizeUnits);
}

void FontDocument::prepareImages(Settings::Presets::Preset* preset, const QStringList& orderedKeys,
                                 QMap<QString, Parsing::ParsedImageData*>* images, Parsing::TagsList& tags) const
{
  DataContainer* data = this->dataContainer();

  // collect ParsedImageData
  {
    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      const QString key = it.next();
      QImage image = QImage(*data->image(key));

      Parsing::ParsedImageData* data = new Parsing::ParsedImageData(preset, &image, tags);
      images->insert(key, data);
    }
  }

  // collect other info
  {
    bool useBom = preset->font()->bom();
    QString encoding = preset->font()->encoding();

    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      QString key = it.next();

      Parsing::ParsedImageData* imageData = images->value(key);

      if (imageData != nullptr) {
        quint64 charCode;
        QString charCodeStr;

        if (this->getCharCode(key, encoding, useBom, charCode, charCodeStr) ||
            !preset->font()->skipMissingCharacters()) {
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterCode, charCodeStr);
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterText,
                                         this->escapeUserCharacters(preset, key));
        }
      }
    }
  }

  // find duplicates
  {
    // map of same character keys by hash
    QMap<uint, Parsing::ParsedImageData*> similarMap;

    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      QString key = it.next();

      Parsing::ParsedImageData* imageData = images->value(key);

      if (imageData != nullptr) {
        // detect same characters
        Parsing::ParsedImageData* similarImageData = similarMap.value(imageData->hash(), nullptr);

        if (similarImageData != nullptr) {
          QString similarCode = similarImageData->tags()->tagValue(Parsing::TagsList::Tag::OutputCharacterCode);
          QString similarText = similarImageData->tags()->tagValue(Parsing::TagsList::Tag::OutputCharacterText);
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterCodeSimilar, similarCode);
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterTextSimilar, similarText);
        } else {
          similarMap.insert(imageData->hash(), imageData);
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterCodeSimilar, QString());
          imageData->tags()->setTagValue(Parsing::TagsList::Tag::OutputCharacterTextSimilar, QString());
        }
      }
    }
  }

  // find min/max codes
  {
    quint64 charCodeMin = std::numeric_limits<quint64>::max();
    quint64 charCodeMax = std::numeric_limits<quint64>::min();
    QString charCodeMinStr = QString();
    QString charCodeMaxStr = QString();
    bool useBom = preset->font()->bom();
    QString encoding = preset->font()->encoding();

    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      QString key = it.next();

      Parsing::ParsedImageData* imageData = images->value(key);

      if (imageData != nullptr) {
        quint64 charCode;
        QString charCodeStr;

        if (this->getCharCode(key, encoding, useBom, charCode, charCodeStr) ||
            !preset->font()->skipMissingCharacters()) {
          if (charCode < charCodeMin) {
            charCodeMin = charCode;
            charCodeMinStr = charCodeStr;
          }

          if (charCode > charCodeMax) {
            charCodeMax = charCode;
            charCodeMaxStr = charCodeStr;
          }
        }
      }
    }

    tags.setTagValue(Parsing::TagsList::Tag::OutputCharacterCodeMin, QString("%1").arg(charCodeMinStr));
    tags.setTagValue(Parsing::TagsList::Tag::OutputCharacterCodeMax, QString("%1").arg(charCodeMaxStr));
  }
}

bool FontDocument::getCharCode(const QString& key, const QString& encoding, bool bom, quint64& resultCode,
                               QString& resultCodeStr) const
{
  QTextCodec* codec = QTextCodec::codecForName(encoding.toLatin1());
  QChar ch = key.at(0);

  QByteArray codeArray = codec->fromUnicode(&ch, 1);

  quint64 code = 0;

  for (int i = 0; i < codeArray.count() && i < 8; i++) {
    code = code << 8;
    code |= (quint8)codeArray.at(i);
  }

  if (!codec->canEncode(ch)) {
    code = ch.unicode();
  }

  if (encoding.contains("UTF-16")) {
    // reorder bytes
    quint64 a = ((code & 0x000000000000ff00ULL) >> 8) | ((code & 0x00000000000000ffULL) << 8);
    code &= 0xffffffffffff0000ULL;
    code |= a;

    if (bom) {
      // 0xfeff00c1
      resultCodeStr = QString("%1").arg(code, 8, 16, QChar('0'));
    } else {
      // 0x00c1
      code &= 0x000000000000ffffULL;
      resultCodeStr = QString("%1").arg(code, 4, 16, QChar('0'));
    }
  } else if (encoding.contains("UTF-32")) {
    // reorder bytes
    quint64 a = ((code & 0x00000000ff000000ULL) >> 24) | ((code & 0x0000000000ff0000ULL) >> 8) |
                ((code & 0x000000000000ff00ULL) << 8) | ((code & 0x00000000000000ffULL) << 24);
    code &= 0xffffffff00000000ULL;
    code |= a;

    if (bom) {
      // 0x0000feff000000c1
      resultCodeStr = QString("%1").arg(code, 16, 16, QChar('0'));
    } else {
      // 0x000000c1
      code &= 0x00000000ffffffffULL;
      resultCodeStr = QString("%1").arg(code, 8, 16, QChar('0'));
    }
  } else {
    resultCodeStr = QString("%1").arg(code, codeArray.count() * 2, 16, QChar('0'));
  }

  resultCode = code;

  return codec->canEncode(ch);
}

bool caseInsensitiveLessThan(const QString& s1, const QString& s2) { return s1.toLower() < s2.toLower(); }

bool caseInsensitiveMoreThan(const QString& s1, const QString& s2) { return s1.toLower() > s2.toLower(); }

const QStringList FontDocument::sortKeysWithEncoding(const QStringList& keys, Settings::Presets::Preset* preset) const
{
  bool useBom = preset->font()->bom();
  QString encoding = preset->font()->encoding();
  Settings::Presets::CharactersSortOrder order = preset->font()->sortOrder();

  if (order == Parsing::Conversion::Options::CharactersSortOrder::None) {
    return keys;
  }

  QMap<QString, QString> map;

  QListIterator<QString> it(keys);
  it.toFront();

  while (it.hasNext()) {
    const QString key = it.next();
    quint64 charCode;
    QString charCodeStr;

    if (this->getCharCode(key, encoding, useBom, charCode, charCodeStr) || !preset->font()->skipMissingCharacters()) {
      map.insert(charCodeStr, key);
    }
  }

  QStringList hexCodes = map.keys();

  switch (order) {
    case Parsing::Conversion::Options::CharactersSortOrder::Ascending:
      std::sort(hexCodes.begin(), hexCodes.end(), caseInsensitiveLessThan);
      break;

    case Parsing::Conversion::Options::CharactersSortOrder::Descending:
      std::sort(hexCodes.begin(), hexCodes.end(), caseInsensitiveMoreThan);
      break;

    default:
      break;
  }

  QStringList result;
  it = QListIterator<QString>(hexCodes);
  it.toFront();

  while (it.hasNext()) {
    const QString key = it.next();
    result.append(map.value(key));
  }

  return result;
}

const QString FontDocument::escapeUserCharacters(Settings::Presets::Preset* preset, const QString& value) const
{
  const QString& chars = preset->font()->escapedCharacters();

  if (chars.isEmpty()) {
    return value;
  }

  const QString& prefix = preset->font()->escapePrefix();
  const QString& suffix = preset->font()->escapeSuffix();

  if (prefix.isEmpty() && suffix.isEmpty()) {
    return value;
  }

  QString result;

  foreach (QChar ch, value) {
    if (chars.contains(ch)) {
      result.append(prefix);
      result.append(ch);
      result.append(suffix);
    } else {
      result.append(ch);
    }
  }

  return result;
}

void FontDocument::mon_container_dataChanged(bool historyStateMoved)
{
  if (this->mNestedChangesCounter == 0) {
    if (!historyStateMoved) {
      this->mContainer->stateSave();
    }

    emit this->documentChanged();
  }
}

} // namespace Containers
} // namespace Data
