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

#include "editortabimage.h"
#include "ui_editortabimage.h"
//-----------------------------------------------------------------------------
#include <QHBoxLayout>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QBuffer>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

#include "widgetbitmapeditor.h"
#include "datacontainer.h"
#include "parser.h"
#include "tags.h"
#include "statusdata.h"
//-----------------------------------------------------------------------------
const QString EditorTabImage::DefaultKey = QString("default");
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mContainer = new DataContainer(this);

    this->mEditor = new WidgetBitmapEditor(this);
    layout->addWidget(this->mEditor);

    this->connect(this->mContainer, SIGNAL(imagesChanged()), SLOT(mon_container_imagesChanged()));
    this->connect(this->mEditor, SIGNAL(imageChanged()), SLOT(mon_editor_imageChanged()));
    this->connect(this->mEditor, SIGNAL(mouseMove(QPoint)), SLOT(mon_editor_mouseMove(QPoint)));
    this->connect(this->mEditor, SIGNAL(scaleSchanged(int)), SLOT(mon_editor_scaleChanged(int)));

    this->setDocumentName(tr("Image", "new image name"));
    this->setFileName("");
    this->setConvertedFileName("");
    this->setChanged(false);

    this->initStatusData();

    this->updateSelectedImage();
}
//-----------------------------------------------------------------------------
EditorTabImage::~EditorTabImage()
{
    delete ui;
}
//-----------------------------------------------------------------------------
void EditorTabImage::changeEvent(QEvent *e)
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
void EditorTabImage::setFileName(const QString &value)
{
    if (this->fileName() != value)
    {
        this->mContainer->setInfo("filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
QString EditorTabImage::convertedFileName() const
{
    QVariant result = this->mContainer->info("converted filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
void EditorTabImage::setConvertedFileName(const QString &value)
{
    if (this->convertedFileName() != value)
    {
        this->mContainer->setInfo("converted filename", QVariant(value));
    }
}
//-----------------------------------------------------------------------------
void EditorTabImage::initStatusData()
{
    this->mStatusData = new StatusData(this);
    this->connect(this->mStatusData, SIGNAL(changed()), SIGNAL(statusChanged()));
    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::updateSelectedImage()
{
    const QImage *image = this->mContainer->image(DefaultKey);
    this->mEditor->setImage(image);

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_container_imagesChanged()
{
    this->updateSelectedImage();
    this->setChanged(true);
    emit this->documentChanged(true, this->documentName(), this->fileName());
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_imageChanged()
{
    this->beginChanges();

    const QImage *image = this->mEditor->image();
    this->mContainer->setImage(DefaultKey, image);

    this->endChanges();
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_mouseMove(QPoint point)
{
    if (point.x() >= 0 && point.y() >= 0)
    {
        this->mStatusData->setData(StatusData::MouseCoordinates, QVariant(point));
    }
    else
    {
        this->mStatusData->removeData(StatusData::MouseCoordinates);
    }
}
//-----------------------------------------------------------------------------
void EditorTabImage::mon_editor_scaleChanged(int scale)
{
    this->mStatusData->setData(StatusData::Scale, QVariant(scale));
}
//-----------------------------------------------------------------------------
bool EditorTabImage::load(const QString &fileName)
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
                QDomNode n = root.firstChild();
                while( !n.isNull() )
                {
                  QDomElement e = n.toElement();
                  if( !e.isNull() )
                  {
                    if( e.tagName() == "picture" )
                    {
                      QString str = e.text();
                      QByteArray ba = QByteArray::fromBase64(str.toAscii());
                      QBuffer buffer(&ba);
                      QImage image;
                      image.load(&buffer, "PNG");
                      this->mContainer->setImage(DefaultKey, &image);
                      result = true;
                    }
                    else if( e.tagName() == "converted" )
                    {
                        converted = e.text();
                    }
                  }

                  n = n.nextSibling();
                }
            }
        }
        file.close();

        this->mEditor->setImage(this->mContainer->image(DefaultKey));

        this->setFileName(fileName);
        this->setConvertedFileName(converted);
        this->setChanged(false);

        this->updateStatus();
    }
    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabImage::save(const QString &fileName)
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
    nodeConverted.appendChild(doc.createTextNode(this->convertedFileName()));

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
bool EditorTabImage::changed() const
{
    bool result = this->mContainer->info("data changed").toBool();
    return result;
}
//-----------------------------------------------------------------------------
void EditorTabImage::setChanged(bool value)
{
    if (this->changed() != value)
    {
        this->mContainer->setInfo("data changed", value);
        emit this->documentChanged(value, this->documentName(), this->fileName());
    }
}
//-----------------------------------------------------------------------------
QString EditorTabImage::fileName() const
{
    QVariant result = this->mContainer->info("filename");
    return result.toString();
}
//-----------------------------------------------------------------------------
QString EditorTabImage::documentName() const
{
    QVariant result = this->mContainer->info("document name");
    return result.toString();
}
//-----------------------------------------------------------------------------
void EditorTabImage::setDocumentName(const QString &value)
{
    if (this->documentName() != value)
    {
        this->mContainer->setInfo("document name", value);
        this->setChanged(true);
    }
}
//-----------------------------------------------------------------------------
DataContainer *EditorTabImage::dataContainer()
{
    return this->mContainer;
}
//-----------------------------------------------------------------------------
QStringList EditorTabImage::selectedKeys() const
{
    QStringList result;

    result << DefaultKey;

    return result;
}
//-----------------------------------------------------------------------------
void EditorTabImage::convert(bool request)
{
    Tags tags;

    if (!this->fileName().isEmpty())
        tags.setTagValue(Tags::DocumentFilename, this->fileName());
    else
        tags.setTagValue(Tags::DocumentFilename, "unsaved");

    tags.setTagValue(Tags::DocumentName, this->documentName());
    tags.setTagValue(Tags::DocumentNameWithoutSpaces, this->documentName().remove(QRegExp("\\W", Qt::CaseInsensitive)));

    tags.setTagValue(Tags::DocumentDataType, "image");

    Parser parser(this, Parser::TypeImage);
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
void EditorTabImage::updateStatus()
{
    const QImage *currentImage = this->mContainer->image(DefaultKey);
    this->mStatusData->setData(StatusData::ImageSize, QVariant(currentImage->size()));

    this->mStatusData->setData(StatusData::Scale, QVariant(this->mEditor->scale()));
}
//-----------------------------------------------------------------------------
StatusData *EditorTabImage::statusData() const
{
    return this->mStatusData;
}
//-----------------------------------------------------------------------------
void EditorTabImage::beginChanges()
{
    if (!this->mContainer->historyInitialized())
    {
        this->mContainer->historyInit();
    }
}
//-----------------------------------------------------------------------------
void EditorTabImage::endChanges()
{
    this->mContainer->stateSave();
}
//-----------------------------------------------------------------------------
bool EditorTabImage::canUndo()
{
    return this->mContainer->canUndo();
}
//-----------------------------------------------------------------------------
bool EditorTabImage::canRedo()
{
    return this->mContainer->canRedo();
}
//-----------------------------------------------------------------------------
void EditorTabImage::undo()
{
    this->mContainer->stateUndo();
    this->updateSelectedImage();

    emit this->documentChanged(this->changed(), this->documentName(), this->fileName());

    this->updateStatus();
}
//-----------------------------------------------------------------------------
void EditorTabImage::redo()
{
    this->mContainer->stateRedo();
    this->updateSelectedImage();

    emit this->documentChanged(this->changed(), this->documentName(), this->fileName());

    this->updateStatus();
}
//-----------------------------------------------------------------------------
/*
 Storage data format:
<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="documentName">
    <picture format="png">
        Base64 string
    </picture>
</data>

<?xml version="1.0" encoding="utf-8"?>
<data type="image" name="Image">
    <converted>/tmp/font.c</converted>
    <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAAsTAAALEwEAmpwYAAAARklEQVQYlYWPSQrAMAwDpZAX5x/Nl6eHgLMQt8IniRGWAeWqkux+zaD5my5B2z0uzHq0XegjW1+ZdLhbB4mkB0iHjY6fYS/sJjZR2Wu+lAAAAABJRU5ErkJggg==</picture>
</data>

 */
