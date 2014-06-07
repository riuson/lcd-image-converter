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
#include "datacontainer.h"
#include "tags.h"
#include "parser.h"
#include "dialogfontchanged.h"
#include "bitmapeditoroptions.h"
#include "fonthelper.h"
#include "preset.h"
//-----------------------------------------------------------------------------
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
        this->beginChanges();

        QDomDocument doc;
        QString errorMsg;
        QString converted;
        int errorColumn, errorLine;

        this->mContainer->blockSignals(true);

        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
            QDomElement root = doc.documentElement();
            if (root.tagName() == "data")
            {
                this->setDocumentName(root.attribute("name", fileName));

                QString fontFamily;
                bool bold = false, italic = false;
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
                        else if (e.tagName() == "bold")
                        {
                            bold = (e.text() == "true");
                        }
                        else if (e.tagName() == "italic")
                        {
                            italic = (e.text() == "true");
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
                QFont font = fonts.font(fontFamily, "Regular", size);
                font.setPixelSize(size);
                font.setBold(bold);
                font.setItalic(italic);
                this->setUsedFont(font);

                this->setBold(bold);
                this->setItalic(italic);
                this->setMonospaced(monospaced);
                this->setAntialiasing(antialiasing);
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
//-----------------------------------------------------------------------------
bool FontDocument::save(const QString &fileName)
{
    bool result = false;

    QDomDocument doc;
    QDomProcessingInstruction procInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");
    doc.appendChild(procInstruction);

    QDomElement nodeRoot = doc.createElement("data");
    doc.appendChild(nodeRoot);

    QString chars, fontFamily;
    int size;
    bool monospaced, antialiasing, bold, italic;
    this->fontCharacters(&chars, &fontFamily, &bold, &italic, &size, &monospaced, &antialiasing);

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

    // monospaced or proportional
    QDomElement nodeWidthType = doc.createElement("widthType");
    nodeRoot.appendChild(nodeWidthType);
    if (monospaced)
        nodeWidthType.appendChild(doc.createTextNode("monospaced"));
    else
        nodeWidthType.appendChild(doc.createTextNode("proportional"));

    // bold
    QDomElement nodeBold = doc.createElement("bold");
    nodeRoot.appendChild(nodeBold);
    if (bold)
        nodeBold.appendChild(doc.createTextNode("true"));
    else
        nodeBold.appendChild(doc.createTextNode("false"));

    // italic
    QDomElement nodeItalic = doc.createElement("italic");
    nodeRoot.appendChild(nodeItalic);
    if (italic)
        nodeItalic.appendChild(doc.createTextNode("true"));
    else
        nodeItalic.appendChild(doc.createTextNode("false"));

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
    while (it.hasNext())
    {
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
    if (file.open(QIODevice::WriteOnly))
    {
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
//-----------------------------------------------------------------------------
bool FontDocument::changed() const
{
    bool result = this->mContainer->changed();
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
    this->mContainer->setInfo("document name", value);
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
    this->mContainer->setInfo("converted filename", QVariant(value));
}
//-----------------------------------------------------------------------------
DataContainer *FontDocument::dataContainer()
{
    return this->mContainer;
}
//-----------------------------------------------------------------------------
QString FontDocument::convert(Preset *preset)
{
    Tags tags;

    if (!this->documentFilename().isEmpty())
        tags.setTagValue(Tags::DocumentFilename, this->documentFilename());
    else
        tags.setTagValue(Tags::DocumentFilename, "unsaved");

    tags.setTagValue(Tags::DocumentName, this->documentName());
    tags.setTagValue(Tags::DocumentNameWithoutSpaces, this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));

    QString chars, fontFamily;
    int size;
    bool monospaced, antialiasing, bold, italic;
    this->fontCharacters(&chars, &fontFamily, &bold, &italic, &size, &monospaced, &antialiasing);

    tags.setTagValue(Tags::DocumentDataType, "font");
    tags.setTagValue(Tags::FontFamily, fontFamily);
    tags.setTagValue(Tags::FontSize, QString("%1").arg(size));
    tags.setTagValue(Tags::FontBold, bold ? "yes" : "no");
    tags.setTagValue(Tags::FontItalic, italic ? "yes" : "no");
    tags.setTagValue(Tags::FontString, FontHelper::escapeControlChars(chars));
    tags.setTagValue(Tags::FontAntiAliasing, antialiasing ? "yes" : "no");
    tags.setTagValue(Tags::FontWidthType, monospaced ? "monospaced" : "proportional");

    Parser parser(Parser::TypeFont, preset, this);
    QString result = parser.convert(this, tags);
/*
    // converter output file name
    QString outputFileName = this->outputFilename();

    // if file name not specified, show dialog
    bool filenameNotSpecified = outputFileName.isEmpty();

    // show dialog
    if (request || filenameNotSpecified)
    {
        QFileDialog dialog(qobject_cast<QWidget *>(this->parent()));
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(outputFileName);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setNameFilter(tr("C Files (*.c);;All Files (*.*)"));
        dialog.setDefaultSuffix(QString("c"));
        dialog.setWindowTitle(tr("Save result file as"));

        if (filenameNotSpecified)
        {
            dialog.selectFile(this->documentName());
        }
        else
        {
            dialog.selectFile(outputFileName);
        }

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
*/
    return result;
}
//-----------------------------------------------------------------------------
void FontDocument::beginChanges()
{
    if (!this->mContainer->historyInitialized())
    {
        this->mContainer->historyInit();
    }

    this->mNestedChangesCounter++;
}
//-----------------------------------------------------------------------------
void FontDocument::endChanges(bool suppress)
{
    if (this->mNestedChangesCounter > 0)
    {
        if (--this->mNestedChangesCounter == 0)
        {
            bool changed = this->mContainer->changed();

            if (suppress)
            {
                this->mContainer->setChanged(false);
                changed = false;
            }

            if (changed)
            {
                this->mContainer->stateSave();
            }

            emit this->documentChanged();
        }
    }
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
                                  bool *_bold,
                                  bool *_italic,
                                  int *_size,
                                  bool *_monospaced,
                                  bool *_antialiasing)
{
    QStringList charList(this->mContainer->keys());
    *chars = charList.join("");
    *fontFamily = this->usedFont().family();
    *_size = this->usedFont().pixelSize();
    *_bold = this->bold();
    *_italic = this->italic();
    *_monospaced = this->monospaced();
    *_antialiasing = this->antialiasing();
}
//-----------------------------------------------------------------------------
void FontDocument::setFontCharacters(const QString &chars,
                                     const QString &fontFamily,
                                     const bool &_bold,
                                     const bool &_italic,
                                     const int _size,
                                     const bool _monospaced,
                                     const bool _antialiasing)
{
    QFontDatabase fonts;

    bool regenerateAll = false;

    this->mContainer->blockSignals(true);

    if (this->mContainer->count() > 1)
    {
        if (this->usedFont().family() != fontFamily ||
            this->bold() != _bold ||
            this->italic() != _italic ||
            this->usedFont().pixelSize() != _size ||
            this->monospaced() != _monospaced ||
            this->antialiasing() != _antialiasing)
        {
            DialogFontChanged dialog(qobject_cast<QWidget *>(this->parent()));
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

    this->beginChanges();

    // create font with specified parameters
    QFont fontNew = fonts.font(fontFamily, "Regular", _size);
    fontNew.setBold(_bold);
    fontNew.setItalic(_italic);
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
        this->setBold(_bold);
        this->setItalic(_italic);
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
                this->mContainer->removeImage(a);
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
    QStringList userOrdered;
    for (int i = 0; i < chars.count(); i++)
    {
        QString key = QString(chars.at(i));
        userOrdered.append(key);

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

    this->mContainer->reorderTo(&userOrdered);

    this->mContainer->blockSignals(false);

    this->endChanges(false);
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
bool FontDocument::bold() const
{
    return this->mContainer->info("bold").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setBold(bool value)
{
    this->mContainer->setInfo("bold", value);
}
//-----------------------------------------------------------------------------
bool FontDocument::italic() const
{
    return this->mContainer->info("italic").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setItalic(bool value)
{
    this->mContainer->setInfo("italic", value);
}
//-----------------------------------------------------------------------------
bool FontDocument::monospaced() const
{
    return this->mContainer->info("monospaced").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setMonospaced(const bool value)
{
    this->mContainer->setInfo("monospaced", value);
}
//-----------------------------------------------------------------------------
bool FontDocument::antialiasing() const
{
    return this->mContainer->info("antialiasing").toBool();
}
//-----------------------------------------------------------------------------
void FontDocument::setAntialiasing(const bool value)
{
    this->mContainer->setInfo("antialiasing", value);
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

    // check for abnormal size
    if ((charWidth > charHeight * 100) || (charWidth == 0))
    {
        if (value.isNull() || !value.isPrint())
        {
            charWidth = 1;
        }
    }

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
void FontDocument::mon_container_dataChanged(bool historyStateMoved)
{
    if (this->mNestedChangesCounter == 0)
    {
        if (!historyStateMoved)
        {
            this->mContainer->stateSave();
        }

        emit this->documentChanged();
    }
}
//-----------------------------------------------------------------------------
