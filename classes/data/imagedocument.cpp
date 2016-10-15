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

#include "imagedocument.h"

#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QWidget>
#include "datacontainer.h"
#include "parser.h"
#include "tags.h"
#include "statusdata.h"
#include "preset.h"
#include "parsedimagedata.h"

const QString ImageDocument::DefaultKey = QString("default");

ImageDocument::ImageDocument(QObject *parent) :
  QObject(parent)
{
  this->mContainer = new DataContainer(this);
  this->connect(this->mContainer, SIGNAL(dataChanged(bool)), SLOT(mon_container_dataChanged(bool)));

  this->mNestedChangesCounter = 0;

  this->beginChanges();

  QImage *image = new QImage(":/images/template");
  this->mContainer->setImage(ImageDocument::DefaultKey, image);
  delete image;

  this->setDocumentName(QString("Image"));
  this->setDocumentFilename("");
  this->setOutputFilename("");

  this->endChanges(true);
  this->mContainer->historyInit();
}

ImageDocument::~ImageDocument()
{
}

bool ImageDocument::load(const QString &fileName)
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
        QDomNode n = root.firstChild();

        while (!n.isNull()) {
          QDomElement e = n.toElement();

          if (!e.isNull()) {
            if (e.tagName() == "picture") {
              QString str = e.text();
              QByteArray ba = QByteArray::fromBase64(str.toLatin1());
              QBuffer buffer(&ba);
              QImage image;
              image.load(&buffer, "PNG");
              this->mContainer->setImage(DefaultKey, &image);
              result = true;
            } else if (e.tagName() == "converted") {
              converted = e.text();
            }
          }

          n = n.nextSibling();
        }
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

bool ImageDocument::save(const QString &fileName)
{
  bool result = false;
  QDomDocument doc;
  QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
  doc.appendChild(procInstruction);

  QDomElement nodeRoot = doc.createElement("data");
  doc.appendChild(nodeRoot);

  nodeRoot.setAttribute("type", "image");
  nodeRoot.setAttribute("name", this->documentName());

  // image data
  QDomElement nodePicture = doc.createElement("picture");
  nodeRoot.appendChild(nodePicture);

  nodePicture.setAttribute("format", "png");

  QByteArray ba;
  QBuffer buffer(&ba);
  buffer.open(QIODevice::WriteOnly);
  this->mContainer->image(DefaultKey)->save(&buffer, "PNG");
  QString data = ba.toBase64();

  QDomText nodeData = doc.createTextNode(data);
  nodePicture.appendChild(nodeData);

  // converted file name
  QDomElement nodeConverted = doc.createElement("converted");
  nodeRoot.appendChild(nodeConverted);
  nodeConverted.appendChild(doc.createTextNode(this->outputFilename()));

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

bool ImageDocument::changed() const
{
  bool result = this->mContainer->changed();
  return result;
}

QString ImageDocument::documentFilename() const
{
  QVariant result = this->mContainer->commonInfo("filename");
  return result.toString();
}

QString ImageDocument::documentName() const
{
  QVariant result = this->mContainer->commonInfo("document name");
  return result.toString();
}

void ImageDocument::setDocumentName(const QString &value)
{
  this->mContainer->setCommonInfo("document name", value);
}

QString ImageDocument::outputFilename() const
{
  QVariant result = this->mContainer->commonInfo("converted filename");
  return result.toString();
}

void ImageDocument::setOutputFilename(const QString &value)
{
  this->mContainer->setCommonInfo("converted filename", QVariant(value));
}

DataContainer *ImageDocument::dataContainer() const
{
  return this->mContainer;
}

QString ImageDocument::convert(Preset *preset)
{
  Tags tags;

  if (!this->documentFilename().isEmpty()) {
    tags.setTagValue(Tags::DocumentFilename, this->documentFilename());
  } else {
    tags.setTagValue(Tags::DocumentFilename, "unsaved");
  }

  tags.setTagValue(Tags::DocumentName, this->documentName());
  tags.setTagValue(Tags::DocumentNameWithoutSpaces, this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));

  tags.setTagValue(Tags::DocumentDataType, "image");

  QMap<QString, ParsedImageData *> images;
  this->prepareImages(preset, &images, tags);
  Parser parser(Parser::TypeImage, preset, this);
  QString result = parser.convert(this, this->dataContainer()->keys(), &images, tags);

  return result;
}

void ImageDocument::beginChanges()
{
  if (!this->mContainer->historyInitialized()) {
    this->mContainer->historyInit();
  }

  this->mNestedChangesCounter++;
}

void ImageDocument::endChanges(bool suppress)
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

bool ImageDocument::canUndo()
{
  return this->mContainer->canUndo();
}

bool ImageDocument::canRedo()
{
  return this->mContainer->canRedo();
}

void ImageDocument::undo()
{
  this->mContainer->stateUndo();

  emit this->documentChanged();
}

void ImageDocument::redo()
{
  this->mContainer->stateRedo();

  emit this->documentChanged();
}

void ImageDocument::setDocumentFilename(const QString &value)
{
  if (this->documentFilename() != value) {
    this->mContainer->setCommonInfo("filename", QVariant(value));
  }
}

void ImageDocument::prepareImages(Preset *preset, QMap<QString, ParsedImageData *> *images, const Tags &tags) const
{
  DataContainer *data = this->dataContainer();

  // collect ParsedImageData
  {
    QListIterator<QString> it(data->keys());
    it.toFront();

    while (it.hasNext()) {
      const QString key = it.next();
      QImage image = QImage(*data->image(key));

      ParsedImageData *data = new ParsedImageData(preset, &image, tags);
      images->insert(key, data);
    }
  }
}

void ImageDocument::mon_container_dataChanged(bool historyStateMoved)
{
  if (this->mNestedChangesCounter == 0) {
    if (!historyStateMoved) {
      this->mContainer->stateSave();
    }

    emit this->documentChanged();
  }
}

