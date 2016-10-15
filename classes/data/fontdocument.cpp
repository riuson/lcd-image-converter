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
#include <QFile>
#include <QDomDocument>
#include <QBuffer>
#include <QImage>
#include <QFontDatabase>
#include <QModelIndexList>
#include <QModelIndex>
#include <QFileDialog>
#include <QTextStream>
#include <QPainter>
#include <QWidget>
#include <QTextCodec>
#include "datacontainer.h"
#include "tags.h"
#include "parser.h"
#include "dialogfontchanged.h"
#include "fonteditoroptions.h"
#include "fonthelper.h"
#include "preset.h"
#include "tfontparameters.h"
#include "fonthelper.h"
#include "parsedimagedata.h"
#include "fontoptions.h"
#include "bitmaphelper.h"

FontDocument::FontDocument(QObject *parent) :
  QObject(parent)
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

FontDocument::~FontDocument()
{
}

bool FontDocument::load(const QString &fileName)
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
        this->setDocumentName(root.attribute("name", fileName));

        QString fontFamily, style;
        int size = 0;
        int ascent = 0, descent = 0;
        bool monospaced = false, antialiasing = false;
        QColor foreground = FontEditorOptions::foreColor();
        QColor background = FontEditorOptions::backColor();

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
              }
            } else if (e.tagName() == "ascent") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                ascent = a;
              }
            } else if (e.tagName() == "descent") {
              bool ok;
              int a = e.text().toInt(&ok);

              if (ok) {
                descent = a;
              }
            } else if (e.tagName() == "widthType") {
              monospaced = (e.text() == "monospaced");
            } else if (e.tagName() == "antialiasing") {
              antialiasing = (e.text() == "true");
            } else if (e.tagName() == "foreground") {
              bool ok;
              QString str = e.text();
              qint32 a = str.toUInt(&ok, 16);

              if (ok) {
                foreground = BitmapHelper::fromRgba(QRgb(a));
              }
            } else if (e.tagName() == "background") {
              bool ok;
              QString str = e.text();
              qint32 a = str.toUInt(&ok, 16);

              if (ok) {
                background = BitmapHelper::fromRgba(QRgb(a));
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
          }
        }

        QFontDatabase fonts;
        QFont font = fonts.font(fontFamily, style, size);
        font.setPixelSize(size);
        this->setUsedFont(font);

        this->setUsedStyle(style);
        this->setMonospaced(monospaced);
        this->setAntialiasing(antialiasing);
        this->setForeground(foreground);
        this->setBackground(background);
        this->setAscent(ascent);
        this->setDescent(descent);
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

bool FontDocument::save(const QString &fileName)
{
  bool result = false;

  QDomDocument doc;
  QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
  doc.appendChild(procInstruction);

  QDomElement nodeRoot = doc.createElement("data");
  doc.appendChild(nodeRoot);

  QString chars;
  tFontParameters parameters;
  this->fontCharacters(&chars, &parameters);

  nodeRoot.setAttribute("type", "font");
  nodeRoot.setAttribute("name", this->documentName());

  //font family
  QDomElement nodeFamily = doc.createElement("family");
  nodeRoot.appendChild(nodeFamily);
  nodeFamily.appendChild(doc.createTextNode(parameters.family));

  // size
  QDomElement nodeSize = doc.createElement("size");
  nodeRoot.appendChild(nodeSize);
  nodeSize.appendChild(doc.createTextNode(QString("%1").arg(parameters.size)));

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
  nodeForeground.appendChild(doc.createTextNode(QString("%1").arg((quint32)parameters.foreground.rgba(), 8, 16, QChar('0'))));

  // background
  QDomElement nodeBackground = doc.createElement("background");
  nodeRoot.appendChild(nodeBackground);
  nodeBackground.appendChild(doc.createTextNode(QString("%1").arg((quint32)parameters.background.rgba(), 8, 16, QChar('0'))));

  // string
  QDomElement nodeString = doc.createElement("string");
  nodeRoot.appendChild(nodeString);
  nodeString.appendChild(doc.createTextNode(FontHelper::escapeControlChars(chars)));

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
    nodeChar.setAttribute("character", FontHelper::escapeControlChars(key));
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

void FontDocument::setDocumentName(const QString &value)
{
  this->mContainer->setCommonInfo("document name", value);
}

QString FontDocument::outputFilename() const
{
  QVariant result = this->mContainer->commonInfo("converted filename");
  return result.toString();
}

void FontDocument::setOutputFilename(const QString &value)
{
  this->mContainer->setCommonInfo("converted filename", QVariant(value));
}

DataContainer *FontDocument::dataContainer() const
{
  return this->mContainer;
}

QString FontDocument::convert(Preset *preset)
{
  Tags tags;

  if (!this->documentFilename().isEmpty()) {
    tags.setTagValue(Tags::DocumentFilename, this->documentFilename());
  } else {
    tags.setTagValue(Tags::DocumentFilename, "unsaved");
  }

  tags.setTagValue(Tags::DocumentName, this->documentName());
  tags.setTagValue(Tags::DocumentNameWithoutSpaces, this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));

  QString chars;
  tFontParameters parameters;
  this->fontCharacters(&chars, &parameters);

  tags.setTagValue(Tags::DocumentDataType, "font");
  tags.setTagValue(Tags::FontFamily, parameters.family);
  tags.setTagValue(Tags::FontSize, QString("%1").arg(parameters.size));
  tags.setTagValue(Tags::FontStyle, parameters.style);
  tags.setTagValue(Tags::FontString, FontHelper::escapeControlChars(chars));
  tags.setTagValue(Tags::FontAntiAliasing, parameters.antiAliasing ? "yes" : "no");
  tags.setTagValue(Tags::FontWidthType, parameters.monospaced ? "monospaced" : "proportional");
  tags.setTagValue(Tags::FontAscent, QString("%1").arg(parameters.ascent));
  tags.setTagValue(Tags::FontDescent, QString("%1").arg(parameters.descent));

  const QStringList orderedKeys = this->sortKeysWithEncoding(this->dataContainer()->keys(), preset);

  QMap<QString, ParsedImageData *> images;
  this->prepareImages(preset, orderedKeys, &images, tags);
  Parser parser(Parser::TypeFont, preset, this);
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

bool FontDocument::canUndo()
{
  return this->mContainer->canUndo();
}

bool FontDocument::canRedo()
{
  return this->mContainer->canRedo();
}

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

void FontDocument::fontCharacters(QString *chars,
                                  tFontParameters *parameters)
{
  QStringList charList(this->mContainer->keys());
  *chars = charList.join("");
  parameters->family = this->usedFont().family();
  parameters->size = this->usedFont().pixelSize();
  parameters->style = this->usedStyle();
  parameters->monospaced = this->monospaced();
  parameters->antiAliasing = this->antialiasing();
  parameters->foreground = this->foreground();
  parameters->background = this->background();
  parameters->ascent = this->ascent();
  parameters->descent = this->descent();
}

void FontDocument::setFontCharacters(const QString &chars,
                                     const tFontParameters &parameters)
{
  QFontDatabase fonts;

  bool regenerateAll = false;

  this->mContainer->blockSignals(true);

  if (this->mContainer->count() > 1) {
    if (this->usedFont().family() != parameters.family ||
        this->usedStyle() != parameters.style ||
        this->usedFont().pixelSize() != parameters.size ||
        this->monospaced() != parameters.monospaced ||
        this->antialiasing() != parameters.antiAliasing ||
        this->foreground() != parameters.foreground ||
        this->background() != parameters.background ||
        this->ascent() != parameters.ascent ||
        this->descent() != parameters.descent) {
      DialogFontChanged dialog(qobject_cast<QWidget *>(this->parent()));

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
  fontNew.setPixelSize(parameters.size);

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
      width = qMax(width, metrics.width(chars.at(i)));
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
      QImage image = FontHelper::drawCharacter(chars.at(i),
                     fontNew,
                     parameters.foreground,
                     parameters.background,
                     width,
                     height,
                     parameters.antiAliasing);
      this->mContainer->setImage(key, new QImage(image));
    }
  }

  this->mContainer->reorderTo(&userOrdered);

  this->mContainer->blockSignals(false);

  this->endChanges(false);
}

void FontDocument::setDocumentFilename(const QString &value)
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

void FontDocument::setUsedFont(const QFont &value)
{
  this->mContainer->setCommonInfo("used font", value);
}

QString FontDocument::usedStyle() const
{
  return this->mContainer->commonInfo("style").toString();
}

void FontDocument::setUsedStyle(const QString &value)
{
  this->mContainer->setCommonInfo("style", value);
}

bool FontDocument::monospaced() const
{
  return this->mContainer->commonInfo("monospaced").toBool();
}

void FontDocument::setMonospaced(const bool value)
{
  this->mContainer->setCommonInfo("monospaced", value);
}

bool FontDocument::antialiasing() const
{
  return this->mContainer->commonInfo("antialiasing").toBool();
}

void FontDocument::setAntialiasing(const bool value)
{
  this->mContainer->setCommonInfo("antialiasing", value);
}

QColor FontDocument::foreground() const
{
  bool ok;
  quint32 rgbValue = this->mContainer->commonInfo("foreground").toUInt(&ok);

  if (ok) {
    return BitmapHelper::fromRgba(QRgb(rgbValue));
  }

  return FontEditorOptions::foreColor();
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
    return BitmapHelper::fromRgba(QRgb(rgbValue));
  }

  return FontEditorOptions::backColor();
}

void FontDocument::setBackground(const QColor value)
{
  this->mContainer->setCommonInfo("background", (quint32)value.rgba());
}

int FontDocument::ascent() const
{
  return this->mContainer->commonInfo("ascent").toInt();
}

void FontDocument::setAscent(int value)
{
  this->mContainer->setCommonInfo("ascent", value);
}

int FontDocument::descent() const
{
  return this->mContainer->commonInfo("descent").toInt();
}

void FontDocument::setDescent(int value)
{
  this->mContainer->setCommonInfo("descent", value);
}

void FontDocument::prepareImages(Preset *preset, const QStringList &orderedKeys, QMap<QString, ParsedImageData *> *images, const Tags &tags) const
{
  DataContainer *data = this->dataContainer();

  // collect ParsedImageData
  {
    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      const QString key = it.next();
      QImage image = QImage(*data->image(key));

      ParsedImageData *data = new ParsedImageData(preset, &image, tags);
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

      ParsedImageData *imageData = images->value(key);

      if (imageData != NULL) {
        QString charCode = this->hexCode(key, encoding, useBom);
        imageData->tags()->setTagValue(Tags::OutputCharacterCode, charCode);
        imageData->tags()->setTagValue(Tags::OutputCharacterText, FontHelper::escapeControlChars(key));
      }
    }
  }

  // find duplicates
  {
    // map of same character keys by hash
    QMap<uint, ParsedImageData *> similarMap;

    QListIterator<QString> it(orderedKeys);
    it.toFront();

    while (it.hasNext()) {
      QString key = it.next();

      ParsedImageData *imageData = images->value(key);

      if (imageData != NULL) {
        // detect same characters
        ParsedImageData *similarImageData = similarMap.value(imageData->hash(), NULL);

        if (similarImageData != NULL) {
          QString similarCode = similarImageData->tags()->tagValue(Tags::OutputCharacterCode);
          QString similarText = similarImageData->tags()->tagValue(Tags::OutputCharacterText);
          imageData->tags()->setTagValue(Tags::OutputCharacterCodeSimilar, similarCode);
          imageData->tags()->setTagValue(Tags::OutputCharacterTextSimilar, similarText);
        } else {
          similarMap.insert(imageData->hash(), imageData);
          imageData->tags()->setTagValue(Tags::OutputCharacterCodeSimilar, QString());
          imageData->tags()->setTagValue(Tags::OutputCharacterTextSimilar, QString());
        }
      }
    }
  }
}

QString FontDocument::hexCode(const QString &key, const QString &encoding, bool bom) const
{
  QString result;
  QTextCodec *codec = QTextCodec::codecForName(encoding.toLatin1());

  QChar ch = key.at(0);
  QByteArray codeArray = codec->fromUnicode(&ch, 1);

  quint64 code = 0;

  for (int i = 0; i < codeArray.count() && i < 8; i++) {
    code = code << 8;
    code |= (quint8)codeArray.at(i);
  }

  if (encoding.contains("UTF-16")) {
    // reorder bytes
    quint64 a =
      ((code & 0x000000000000ff00ULL) >> 8) |
      ((code & 0x00000000000000ffULL) << 8);
    code &= 0xffffffffffff0000ULL;
    code |= a;

    if (bom) {
      // 0xfeff00c1
      result = QString("%1").arg(code, 8, 16, QChar('0'));
    } else {
      // 0x00c1
      code &= 0x000000000000ffffULL;
      result = QString("%1").arg(code, 4, 16, QChar('0'));
    }
  } else if (encoding.contains("UTF-32")) {
    // reorder bytes
    quint64 a =
      ((code & 0x00000000ff000000ULL) >> 24) |
      ((code & 0x0000000000ff0000ULL) >> 8) |
      ((code & 0x000000000000ff00ULL) << 8) |
      ((code & 0x00000000000000ffULL) << 24);
    code &= 0xffffffff00000000ULL;
    code |= a;

    if (bom) {
      // 0x0000feff000000c1
      result = QString("%1").arg(code, 16, 16, QChar('0'));
    } else {
      // 0x000000c1
      code &= 0x00000000ffffffffULL;
      result = QString("%1").arg(code, 8, 16, QChar('0'));
    }
  } else {
    result = QString("%1").arg(code, codeArray.count() * 2, 16, QChar('0'));
  }

  return result;
}

bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
  return s1.toLower() < s2.toLower();
}

bool caseInsensitiveMoreThan(const QString &s1, const QString &s2)
{
  return s1.toLower() > s2.toLower();
}

const QStringList FontDocument::sortKeysWithEncoding(const QStringList &keys, Preset *preset) const
{

  bool useBom = preset->font()->bom();
  QString encoding = preset->font()->encoding();
  CharactersSortOrder order = preset->font()->sortOrder();

  if (order == CharactersSortNone) {
    return keys;
  }

  QMap <QString, QString> map;

  QListIterator<QString> it(keys);
  it.toFront();

  while (it.hasNext()) {
    const QString key = it.next();
    const QString hex = this->hexCode(key, encoding, useBom);
    map.insert(hex, key);
  }

  QStringList hexCodes = map.keys();

  switch (order) {
    case CharactersSortAscending:
      qSort(hexCodes.begin(), hexCodes.end(), caseInsensitiveLessThan);
      break;

    case CharactersSortDescending:
      qSort(hexCodes.begin(), hexCodes.end(), caseInsensitiveMoreThan);
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

void FontDocument::mon_container_dataChanged(bool historyStateMoved)
{
  if (this->mNestedChangesCounter == 0) {
    if (!historyStateMoved) {
      this->mContainer->stateSave();
    }

    emit this->documentChanged();
  }
}

