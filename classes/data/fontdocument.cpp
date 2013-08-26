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
#include "datacontainer.h"
#include "tags.h"
#include "parser.h"
#include "dialogfontchanged.h"
#include "bitmapeditoroptions.h"
//-----------------------------------------------------------------------------
FontDocument::FontDocument(QObject *parent) :
    QObject(parent)
{
    this->mContainer = new DataContainer(this);

    this->setDocumentName(tr("Font", "new font name"));
    this->setDocumentFilename("");
    this->setOutputFilename("");
    this->setChanged(false);
    this->setAntialiasing(false);
    this->setMonospaced(false);
}
//-----------------------------------------------------------------------------
FontDocument::~FontDocument()
{
}
//-----------------------------------------------------------------------------
bool FontDocument::load(const QString &fileName)
{
    bool result = false;

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QDomDocument doc;
        QString errorMsg;
        QString converted;
        int errorColumn, errorLine;
        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
            QDomElement root = doc.documentElement();
            if (root.tagName() == "data")
            {
                this->setDocumentName(root.attribute("name", fileName));

                QString fontFamily, style;
                int size = 0;
                bool monospaced = false, antialiasing = false;

                QDomNode n = root.firstChild();
                while (!n.isNull())
                {
                    QDomElement e = n.toElement();

                    if (!e.isNull())
                    {
                        if (e.tagName() == "family")
                        {
                            fontFamily = e.text();
                        }
                        else if (e.tagName() == "style")
                        {
                            style = e.text();
                        }
                        else if (e.tagName() == "size")
                        {
                            bool ok;
                            int a = e.text().toInt(&ok);
                            if (ok)
                                size = a;
                        }
                        else if (e.tagName() == "widthType")
                        {
                            monospaced = (e.text() == "monospaced");
                        }
                        else if (e.tagName() == "antialiasing")
                        {
                            antialiasing = (e.text() == "true");
                        }
                        else if (e.tagName() == "converted")
                        {
                            converted = e.text();
                        }
                    }

                    n = n.nextSibling();
                }

                QDomNodeList nodesChar = root.elementsByTagName("char");
                for (int i = 0; i < nodesChar.count(); i++)
                {
                    QDomElement e = nodesChar.at(i).toElement();
                    bool ok;
                    int code = e.attribute("code", "none").toInt(&ok, 16);
                    if (ok)
                    {
                        QDomElement nodePicture = e.firstChildElement("picture");
                        if (nodePicture.isElement())
                        {
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
            }
        }
        file.close();

        this->setDocumentFilename(fileName);
        this->setOutputFilename(converted);
        this->setChanged(false);
    }

    return result;
}
//-----------------------------------------------------------------------------
bool FontDocument::save(const QString &fileName)
{
    bool result = false;

    QDomDocument doc;
    QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(procInstruction);

    QDomElement nodeRoot = doc.createElement("data");
    doc.appendChild(nodeRoot);

    QString chars, fontFamily, style;
    int size;
    bool monospaced, antialiasing;
    this->fontCharacters(&chars, &fontFamily, &style, &size, &monospaced, &antialiasing);

    nodeRoot.setAttribute("type", "font");
    nodeRoot.setAttribute("name", this->documentName());

    //font family
    QDomElement nodeFamily = doc.createElement("family");
    nodeRoot.appendChild(nodeFamily);
    nodeFamily.appendChild(doc.createTextNode(fontFamily));

    // size
    QDomElement nodeSize = doc.createElement("size");
    nodeRoot.appendChild(nodeSize);
    nodeSize.appendChild(doc.createTextNode(QString("%1").arg(size)));

    // style
    QDomElement nodeStyle = doc.createElement("style");
    nodeRoot.appendChild(nodeStyle);
    nodeStyle.appendChild(doc.createTextNode(style));

    // monospaced or proportional
    QDomElement nodeWidthType = doc.createElement("widthType");
    nodeRoot.appendChild(nodeWidthType);
    if (monospaced)
        nodeWidthType.appendChild(doc.createTextNode("monospaced"));
    else
        nodeWidthType.appendChild(doc.createTextNode("proportional"));

    // antialiasing
    QDomElement nodeAntialiasing = doc.createElement("antialiasing");
    nodeRoot.appendChild(nodeAntialiasing);
    if (monospaced)
        nodeAntialiasing.appendChild(doc.createTextNode("true"));
    else
        nodeAntialiasing.appendChild(doc.createTextNode("false"));

    // string
    QDomElement nodeString = doc.createElement("string");
    nodeRoot.appendChild(nodeString);
    nodeString.appendChild(doc.createTextNode(chars));

    // converted file name
    QDomElement nodeConverted = doc.createElement("converted");
    nodeRoot.appendChild(nodeConverted);
    nodeConverted.appendChild(doc.createTextNode(this->outputFilename()));

    // chars list
    QDomElement nodeChars = doc.createElement("chars");
    nodeRoot.appendChild(nodeChars);

    QListIterator<QString> it(this->mContainer->keys());
    it.toFront();
    while (it.hasNext())
    {
        QString key = it.next();
        // char
        QDomElement nodeChar = doc.createElement("char");
        nodeChars.appendChild(nodeChar);
        nodeChar.setAttribute("character", key);
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
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        doc.save(stream, 4);

        this->setDocumentFilename(fileName);
        file.close();
        result = true;
        this->setChanged(false);
    }

    return result;
}
//-----------------------------------------------------------------------------
bool FontDocument::changed() const
{
    bool result = this->mContainer->info("data changed").toBool();
    return result;
}
//-----------------------------------------------------------------------------
QString FontDocument::documentFilename() const
{
    QVariant result = this->mContainer->info("filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
QString FontDocument::documentName() const
{
    QVariant result = this->mContainer->info("document name");
    return result.toString();
}
//-----------------------------------------------------------------------------
void FontDocument::setDocumentName(const QString &value)
{
    if (this->documentName() != value)
    {
        this->mContainer->setInfo("document name", value);
        this->setChanged(true);
    }
}
//-----------------------------------------------------------------------------
DataContainer *FontDocument::dataContainer()
{
    return this->mContainer;
}
//-----------------------------------------------------------------------------
void FontDocument::convert(bool request)
{
    Tags tags;

    if (!this->documentFilename().isEmpty())
        tags.setTagValue(Tags::DocumentFilename, this->documentFilename());
    else
        tags.setTagValue(Tags::DocumentFilename, "unsaved");

    tags.setTagValue(Tags::DocumentName, this->documentName());
    tags.setTagValue(Tags::DocumentNameWithoutSpaces, this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));

    QString chars, fontFamily, style;
    int size;
    bool monospaced, antialiasing;
    this->fontCharacters(&chars, &fontFamily, &style, &size, &monospaced, &antialiasing);

    tags.setTagValue(Tags::DocumentDataType, "font");
    tags.setTagValue(Tags::FontFamily, fontFamily);
    tags.setTagValue(Tags::FontSize, QString("%1").arg(size));
    tags.setTagValue(Tags::FontStyle, style);
    tags.setTagValue(Tags::FontString, chars);
    tags.setTagValue(Tags::FontAntiAliasing, antialiasing ? "yes" : "no");
    tags.setTagValue(Tags::FontWidthType, monospaced ? "monospaced" : "proportional");

    Parser parser(this, Parser::TypeFont);
    QString result = parser.convert(this, tags);

    // converter output file name
    QString outputFileName = this->outputFilename();

    // if file name not specified, show dialog
    if (outputFileName.isEmpty())
        request = true;

    // show dialog
    if (request)
    {
        QFileDialog dialog;
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(outputFileName);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("C Files (*.c);;All Files (*.*)"));
        dialog.setDefaultSuffix(QString("c"));
        dialog.setWindowTitle(tr("Save result file as"));
        if (dialog.exec() == QDialog::Accepted)
        {
            outputFileName = dialog.selectedFiles().at(0);
        }
        else
        {
            outputFileName = "";
        }
    }

    // if file name specified, save result
    if (!outputFileName.isEmpty())
    {
        QFile file(outputFileName);
        if (file.open(QFile::WriteOnly))
        {
            file.write(result.toUtf8());
            file.close();

            if (this->outputFilename() != outputFileName)
            {
                this->beginChanges();

                this->setOutputFilename(outputFileName);
                emit this->setChanged(true);

                this->endChanges();
            }
        }
    }
}
//-----------------------------------------------------------------------------
void FontDocument::beginChanges()
{
    if (!this->mContainer->historyInitialized())
    {
        this->mContainer->historyInit();
    }
}
//-----------------------------------------------------------------------------
void FontDocument::endChanges()
{
    this->mContainer->stateSave();
}
//-----------------------------------------------------------------------------
bool FontDocument::canUndo()
{
    return this->mContainer->canUndo();
}
//-----------------------------------------------------------------------------
bool FontDocument::canRedo()
{
    return this->mContainer->canRedo();
}
//-----------------------------------------------------------------------------
void FontDocument::undo()
{
    this->mContainer->stateUndo();

    emit this->documentChanged();
}
//-----------------------------------------------------------------------------
void FontDocument::redo()
{
    this->mContainer->stateRedo();

    emit this->documentChanged();
}
//-----------------------------------------------------------------------------
void FontDocument::fontCharacters(QString *chars,
                                  QString *fontFamily,
                                  QString *_style,
                                  int *_size,
                                  bool *_monospaced,
                                  bool *_antialiasing)
{
    QStringList charList(this->mContainer->keys());
    *chars = charList.join("");
    *fontFamily = this->usedFont().family();
    *_size = this->usedFont().pixelSize();
    *_style = this->usedStyle();
    *_monospaced = this->monospaced();
    *_antialiasing = this->antialiasing();
}
//-----------------------------------------------------------------------------
void FontDocument::setFontCharacters(const QString &chars,
                                     const QString &fontFamily,
                                     const QString &_style,
                                     const int _size,
                                     const bool _monospaced,
                                     const bool _antialiasing)
{
    QFontDatabase fonts;

    bool regenerateAll = false;

    if (this->mContainer->count() > 1)
    {
        if (this->usedFont().family() != fontFamily ||
            this->usedStyle() != _style ||
            this->usedFont().pixelSize() != _size ||
            this->monospaced() != _monospaced ||
            this->antialiasing() != _antialiasing)
        {
            DialogFontChanged dialog;
            if (dialog.exec() == QDialog::Accepted)
            {
                regenerateAll = dialog.regenerateAll();
            }
            else
            {
                return;
            }
        }
    }
    else // for newly created fonts
    {
        regenerateAll = true;
    }

    // create font with specified parameters
    QFont fontNew = fonts.font(fontFamily, _style, _size);
    fontNew.setPixelSize(_size);

    if (_antialiasing)
        fontNew.setStyleStrategy(QFont::PreferAntialias);
    else
        fontNew.setStyleStrategy(QFont::NoAntialias);

    // remove old characters
    if (regenerateAll)
    {
        this->mContainer->clear();

        // save new font
        this->setUsedFont(fontNew);
        this->setUsedStyle(_style);
        this->setMonospaced(_monospaced);
        this->setAntialiasing(_antialiasing);
    }
    else
    {
        // remove characters, which not present in new characters list
        QStringList keys = this->mContainer->keys();
        QListIterator<QString> it(keys);
        it.toFront();
        while(it.hasNext())
        {
            QString a = it.next();
            if (!chars.contains(a))
            {
                this->mContainer->remove(a);
            }
        }
    }

    // find max size
    int width = 0, height = 0;
    if (_monospaced)
    {
        QFontMetrics metrics(fontNew);
        for (int i = 0; i < chars.count(); i++)
        {
            width = qMax(width, metrics.width(chars.at(i)));
        }
        height = metrics.height();
    }

    // generate new characters
    QStringList keys = this->mContainer->keys();
    for (int i = 0; i < chars.count(); i++)
    {
        QString key = QString(chars.at(i));

        // if character not exists, create it
        if (!keys.contains(key))
        {
            keys.append(key);
            QImage image = this->drawCharacter(chars.at(i),
                                               fontNew,
                                               BitmapEditorOptions::color1(),
                                               BitmapEditorOptions::color2(),
                                               width,
                                               height,
                                               _antialiasing);
            this->mContainer->setImage(key, new QImage(image));
        }
    }

    this->setChanged(true);
}
//-----------------------------------------------------------------------------
void FontDocument::setDocumentFilename(const QString &value)
{
    if (this->documentFilename() != value)
    {
        this->mContainer->setInfo("filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
QString FontDocument::outputFilename() const
{
    QVariant result = this->mContainer->info("converted filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
void FontDocument::setOutputFilename(const QString &value)
{
    if (this->outputFilename() != value)
    {
        this->mContainer->setInfo("converted filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
QFont FontDocument::usedFont() const
{
    QVariant var = this->mContainer->info("used font");
    if (!var.isNull())
    {
        QFont result = var.value<QFont>();
        return result;
    }
    return QFont();
}
//-----------------------------------------------------------------------------
void FontDocument::setUsedFont(const QFont &value)
{
    this->mContainer->setInfo("used font", value);
}
//-----------------------------------------------------------------------------
QString FontDocument::usedStyle() const
{
    return this->mContainer->info("style").toString();
}
//-----------------------------------------------------------------------------
void FontDocument::setUsedStyle(const QString &value)
{
    if (this->usedStyle() != value)
    {
        this->mContainer->setInfo("style", value);
    }
}
//-----------------------------------------------------------------------------
bool FontDocument::monospaced() const
{
    return this->mContainer->info("monospaced").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setMonospaced(const bool value)
{
    if (this->monospaced() != value)
    {
        this->mContainer->setInfo("monospaced", value);
    }
}
//-----------------------------------------------------------------------------
bool FontDocument::antialiasing() const
{
    return this->mContainer->info("antialiasing").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setAntialiasing(const bool value)
{
    if (this->antialiasing() != value)
    {
        this->mContainer->setInfo("antialiasing", value);
    }
}
//-----------------------------------------------------------------------------
QImage FontDocument::drawCharacter(const QChar value, const QFont &font, const QColor &foreground, const QColor &background, const int width, const int height, const bool antialiasing)
{
    QFontMetrics fontMetrics(font);

    int charWidth = fontMetrics.width(value);
    int charHeight = fontMetrics.height();

    // fix width of italic style
    QRect r = fontMetrics.boundingRect(QString(value));
    charWidth = qMax(qMax(r.left(), r.right()) + 1, charWidth);

    int imageWidth = width;
    int imageHeight = height;

    if (width == 0 || height == 0)
    {
        imageWidth = charWidth;
        imageHeight = charHeight;
    }
    QImage result(imageWidth, imageHeight, QImage::Format_RGB32);

    QPainter painter(&result);
    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing, antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);

    painter.setPen(foreground);

    painter.fillRect(result.rect(), background);

    painter.drawText((imageWidth / 2) - (charWidth / 2),
                     fontMetrics.ascent(),//+4
                     QString(value));

    return result;
}
//-----------------------------------------------------------------------------
void FontDocument::setChanged(bool value)
{
    this->mContainer->setInfo("data changed", value);
    emit this->documentChanged();
}
//-----------------------------------------------------------------------------
void FontDocument::mon_container_imagesChanged()
{
    this->setChanged(true);
    emit this->documentChanged();
}
//-----------------------------------------------------------------------------
