/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2010 riuson
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

#include "editortabfont.h"
#include "ui_editortabfont.h"
//-----------------------------------------------------------------------------
#include <QSplitter>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFontDatabase>
#include <QPainter>
#include <QImage>
#include <QMessageBox>
#include <QFileDialog>

#include "widgetbitmapeditor.h"
#include "datacontainer.h"
#include "fontcharactersmodel.h"
#include "parser.h"
#include "dialogfontchanged.h"
#include "tags.h"
#include "statusdata.h"
//-----------------------------------------------------------------------------
EditorTabFont::EditorTabFont(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::EditorTabFont)
{
    ui->setupUi(this);

    this->mSplitter = new QSplitter(this);
    this->ui->horizontalLayout->addWidget(this->mSplitter);

    this->mContainer = new DataContainer(this);
    this->mSelectedKey = QString();

    this->mModel = new FontCharactersModel(this->mContainer, this);
    this->ui->tableViewCharacters->setModel(this->mModel);

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    this->connect(selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(selectionChanged(QItemSelection,QItemSelection)));

    this->mEditor = new WidgetBitmapEditor(this);
    this->mSplitter->addWidget(this->mEditor);
    this->mSplitter->addWidget(this->ui->tableViewCharacters);
    this->mSplitter->setChildrenCollapsible(false);

    this->connect(this->mContainer, SIGNAL(imageChanged(QString)), SLOT(mon_container_imageChanged(QString)));
    this->connect(this->mEditor, SIGNAL(imageChanged()), SLOT(mon_editor_imageChanged()));
    this->connect(this->mEditor, SIGNAL(mouseMove(QPoint)), SLOT(mon_editor_mouseMove(QPoint)));

    this->setDocumentName(tr("Font", "new font name"));
    this->setFileName("");
    this->setConvertedFileName("");
    this->setChanged(false);

    this->setAntialiasing(false);
    this->setMonospaced(false);

    this->ui->tableViewCharacters->resizeColumnsToContents();

    this->mStatusData = new StatusData(this);
}
//-----------------------------------------------------------------------------
EditorTabFont::~EditorTabFont()
{
    delete ui;
    delete this->mModel;
}
//-----------------------------------------------------------------------------
void EditorTabFont::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::setFileName(const QString &value)
{
    if (this->fileName() != value)
    {
        this->mContainer->setInfo("filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
QString EditorTabFont::convertedFileName() const
{
    QVariant result = this->mContainer->info("converted filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
void EditorTabFont::setConvertedFileName(const QString &value)
{
    if (this->convertedFileName() != value)
    {
        this->mContainer->setInfo("converted filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
QFont EditorTabFont::usedFont() const
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
void EditorTabFont::setUsedFont(const QFont &value)
{
    this->mContainer->setInfo("used font", value);
}
//-----------------------------------------------------------------------------
QString EditorTabFont::usedStyle() const
{
    return this->mContainer->info("style").toString();
}
//-----------------------------------------------------------------------------
void EditorTabFont::setUsedStyle(const QString &value)
{
    if (this->usedStyle() != value)
    {
        this->mContainer->setInfo("style", value);
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_container_imageChanged(const QString &key)
{
    if (this->mSelectedKey == key)
    {
        const QImage *image = this->mContainer->image(key);
        this->mEditor->setImage(image);
    }
    this->setChanged(true);
    emit this->documentChanged(true, this->documentName(), this->fileName());
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_editor_imageChanged()
{
    this->beginChanges();

    const QImage *image = this->mEditor->image();
    this->mContainer->setImage(this->mSelectedKey, image);
    this->setChanged(true);

    this->endChanges();
}
//-----------------------------------------------------------------------------
void EditorTabFont::mon_editor_mouseMove(QPoint point)
{
    if (point.x() >= 0 && point.y() >= 0)
    {
        this->mStatusData->setData(StatusData::MouseCoordinates, QVariant(point));
    }
    else
    {
        this->mStatusData->removeData(StatusData::MouseCoordinates);
    }
    emit this->statusChanged();
}
//-----------------------------------------------------------------------------
void EditorTabFont::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndex index = this->mModel->index(selectionModel->currentIndex().row(), 0);

        this->mSelectedKey = this->mModel->data(index, Qt::DisplayRole).toString();
        const QImage *image = this->mContainer->image(this->mSelectedKey);
        this->mEditor->setImage(image);

        {
            QList<QVariant> list;
            list.append(QVariant(index.row()));
            list.append(QVariant(this->mContainer->count()));
            QVariant var(list);
            this->mStatusData->setData(StatusData::ImageIndex, var);
        }

        emit this->statusChanged();
    }
}
//-----------------------------------------------------------------------------
bool EditorTabFont::load(const QString &fileName)
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
            if( root.tagName() == "data" )
            {
                this->setDocumentName(root.attribute("name", fileName));

                QString fontFamily, style;
                int size = 0;
                bool monospaced = false, antialiasing = false;

                QDomNode n = root.firstChild();
                while( !n.isNull() )
                {
                    QDomElement e = n.toElement();

                    if( !e.isNull() )
                    {
                        if( e.tagName() == "family" )
                        {
                            fontFamily = e.text();
                        }
                        else if( e.tagName() == "style" )
                        {
                            style = e.text();
                        }
                        else if( e.tagName() == "size" )
                        {
                            bool ok;
                            int a = e.text().toInt(&ok);
                            if (ok)
                                size = a;
                        }
                        else if( e.tagName() == "widthType" )
                        {
                            monospaced = (e.text() == "monospaced");
                        }
                        else if( e.tagName() == "antialiasing" )
                        {
                            antialiasing = (e.text() == "true");
                        }
                        else if( e.tagName() == "converted" )
                        {
                            converted = e.text();
                        }
                    }

                    n = n.nextSibling();
                }

                this->mSelectedKey = QString();
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
                            QByteArray ba = QByteArray::fromBase64(str.toAscii());
                            QBuffer buffer(&ba);
                            QImage image;
                            image.load(&buffer, "PNG");

                            QString key = QString(QChar(code));
                            if (this->mSelectedKey.isNull())
                            {
                                this->mSelectedKey = key;
                            }
                            this->mContainer->setImage(key, &image);
                        }
                    }
                }
                this->mModel->callReset();
                this->ui->tableViewCharacters->resizeColumnsToContents();

                QFontDatabase fonts;
                QFont font = fonts.font(fontFamily, style, size);
                font.setPixelSize(size);
                this->setUsedFont(font);

                this->setUsedStyle(style);
                this->setMonospaced(monospaced);
                this->setAntialiasing(antialiasing);
                this->updateTableFont();
            }
        }
        file.close();

        this->setFileName(fileName);
        this->setConvertedFileName(converted);
        this->mEditor->setImage(this->mContainer->image(this->mSelectedKey));
        this->setChanged(false);
    }

    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabFont::save(const QString &fileName)
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
    nodeConverted.appendChild(doc.createTextNode(this->convertedFileName()));

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

        this->setFileName(fileName);
        file.close();
        result = true;
        this->setChanged(false);
    }

    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabFont::changed() const
{
    bool result = this->mContainer->info("data changed").toBool();
    return result;
}
//-----------------------------------------------------------------------------
void EditorTabFont::setChanged(bool value)
{
    if (this->changed() != value)
    {
        this->mContainer->setInfo("data changed", value);
        emit this->documentChanged(value, this->documentName(), this->fileName());
    }
}
//-----------------------------------------------------------------------------
QString EditorTabFont::fileName() const
{
    QVariant result = this->mContainer->info("filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
QString EditorTabFont::documentName() const
{
    QVariant result = this->mContainer->info("document name");
    return result.toString();
}
//-----------------------------------------------------------------------------
void EditorTabFont::setDocumentName(const QString &value)
{
    if (this->documentName() != value)
    {
        this->mContainer->setInfo("document name", value);
        this->setChanged(true);
    }
}
//-----------------------------------------------------------------------------
DataContainer *EditorTabFont::dataContainer()
{
    return this->mContainer;
}
//-----------------------------------------------------------------------------
const QImage *EditorTabFont::image() const
{
    const QImage *result = this->mContainer->image(this->mSelectedKey);
    return result;
}
//-----------------------------------------------------------------------------
void EditorTabFont::setImage(const QImage *value)
{
    this->mContainer->setImage(this->mSelectedKey, value);
}
//-----------------------------------------------------------------------------
void EditorTabFont::convert(bool request)
{
    Tags tags;

    if (!this->fileName().isEmpty())
        tags.setTagValue(Tags::DocumentFilename, this->fileName());
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
    QString outputFileName = this->convertedFileName();

    // if file name not specified, show dialog
    if (outputFileName.isEmpty())
        request = true;

    // show dialog
    if (request)
    {
        QFileDialog dialog(this);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(outputFileName);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setFilter(tr("C Files (*.c);;All Files (*.*)"));
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

            if (this->convertedFileName() != outputFileName)
            {
                this->beginChanges();

                this->setConvertedFileName(outputFileName);
                emit this->setChanged(true);

                this->endChanges();
            }
        }
    }
}
//-----------------------------------------------------------------------------
StatusData *EditorTabFont::statusData() const
{
    return this->mStatusData;
}
//-----------------------------------------------------------------------------
void EditorTabFont::beginChanges()
{
    if (!this->mContainer->historyInitialized())
    {
        this->mContainer->historyInit();
    }
}
//-----------------------------------------------------------------------------
void EditorTabFont::endChanges()
{
    this->mContainer->stateSave();
}
//-----------------------------------------------------------------------------
bool EditorTabFont::canUndo()
{
    return this->mContainer->canUndo();
}
//-----------------------------------------------------------------------------
bool EditorTabFont::canRedo()
{
    return this->mContainer->canRedo();
}
//-----------------------------------------------------------------------------
void EditorTabFont::undo()
{
    this->mContainer->stateUndo();
    this->setImage(this->image());

    emit this->documentChanged(this->changed(), this->documentName(), this->fileName());
}
//-----------------------------------------------------------------------------
void EditorTabFont::redo()
{
    this->mContainer->stateRedo();
    this->setImage(this->image());

    emit this->documentChanged(this->changed(), this->documentName(), this->fileName());
}
//-----------------------------------------------------------------------------
void EditorTabFont::setFontCharacters(const QString &chars,
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
            DialogFontChanged dialog(this);
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

    QImage emptyImage = QImage();
    this->mEditor->setImage(&emptyImage);

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
                                               this->mEditor->color1(),
                                               this->mEditor->color2(),
                                               width,
                                               height,
                                               _antialiasing);
            this->mContainer->setImage(key, new QImage(image));
        }
    }

    this->setChanged(true);

    this->updateTableFont();
    this->mModel->callReset();
    this->ui->tableViewCharacters->resizeColumnsToContents();

    this->mEditor->setImage(this->mContainer->image(keys.at(0)));
}
//-----------------------------------------------------------------------------
void EditorTabFont::fontCharacters(QString *chars,
                                   QString *fontFamily,
                                   QString *_style,
                                   int *_size,
                                   bool *_monospaced,
                                   bool *_antialiasing)
{
    QStringList charList(this->mContainer->keys());
    //*chars = this->mCharacters;
    *chars = charList.join("");
    *fontFamily = this->usedFont().family();
    *_size = this->usedFont().pixelSize();
    *_style = this->usedStyle();
    *_monospaced = this->monospaced();
    *_antialiasing = this->antialiasing();
}
//-----------------------------------------------------------------------------
const QString EditorTabFont::selectedCharacters() const
{
    QString result;

    QItemSelectionModel *selectionModel = this->ui->tableViewCharacters->selectionModel();
    if (selectionModel->hasSelection())
    {
        QModelIndexList indexes = selectionModel->selectedIndexes();
        for (int i = 0; i < indexes.count(); i++)
        {
            QModelIndex index = this->mModel->index(indexes.at(i).row(), 0);
            QString a = this->mModel->data(index, Qt::DisplayRole).toString();
            if (!result.contains(a))
                result += a;
        }
    }

    return result;
}
//-----------------------------------------------------------------------------
QImage EditorTabFont::drawCharacter(const QChar value,
                                    const QFont &font,
                                    const QColor &foreground,
                                    const QColor &background,
                                    const int width,
                                    const int height,
                                    const bool antialiasing)
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
void EditorTabFont::updateTableFont()
{
    this->mTableFont = QFont(this->usedFont());
    this->mTableFont.setPointSize(11);
    this->mTableFont.setBold(false);
    this->ui->tableViewCharacters->setFont(this->mTableFont);
}
//-----------------------------------------------------------------------------
bool EditorTabFont::monospaced() const
{
    return this->mContainer->info("monospaced").toBool();
}
//-----------------------------------------------------------------------------
void EditorTabFont::setMonospaced(const bool value)
{
    if (this->monospaced() != value)
    {
        this->mContainer->setInfo("monospaced", value);
    }
}
//-----------------------------------------------------------------------------
bool EditorTabFont::antialiasing() const
{
    return this->mContainer->info("antialiasing").toBool();
}
//-----------------------------------------------------------------------------
void EditorTabFont::setAntialiasing(const bool value)
{
    if (this->antialiasing() != value)
    {
        this->mContainer->setInfo("antialiasing", value);
    }
}
//-----------------------------------------------------------------------------
/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="font" name="Font">
    <family>Ubuntu</family>
    <size>12</size>
    <style>Normal</style>
    <widthType>proportional</widthType>
    <antialiasing>false</antialiasing>
    <string> !"#$%&amp;'()*+,-./0123456789:;&lt;=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~АБВ</string>
    <converted>/tmp/font.c</converted>
    <chars>
        <char character=" " code="0020">
            <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAQAAAATCAIAAAA4QDsKAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAFUlEQVQImWP8//8/AwwwMSCBYc0BAO+LAyPoIK0eAAAAAElFTkSuQmCC</picture>
        </char>
    </chars>
</data>
 */
