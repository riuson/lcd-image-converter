/*
 * LCD Image Converter. Converts images and fonts for embedded applciations.
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

#include "widgetbitmapeditor.h"
#include "bitmapcontainer.h"
//-----------------------------------------------------------------------------
EditorTabImage::EditorTabImage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditorTabImage)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    this->setLayout(layout);

    this->mContainer = new BitmapContainer(this);

    this->mEditor = new WidgetBitmapEditor(this->mContainer, this);
    layout->addWidget(this->mEditor);


    this->connect(this->mEditor, SIGNAL(dataChanged()), SLOT(mon_editor_dataChanged()));

    this->mDocumentName = tr("Image", "new image name");
    this->mFileName = "";
    this->mDataChanged = false;
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
void EditorTabImage::mon_editor_dataChanged()
{
    this->mDataChanged = true;
    emit this->dataChanged();
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
        int errorColumn, errorLine;
        if (doc.setContent(&file, &errorMsg, &errorLine, &errorColumn))
        {
            QDomElement root = doc.documentElement();
            if( root.tagName() == "data" )
            {
                this->mDocumentName = root.attribute("name", fileName);
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
                      this->mContainer->setImage("default", &image);
                      result = true;
                      break;
                    }
                  }

                  n = n.nextSibling();
                }
            }
        }
        file.close();

        this->mFileName = fileName;
        this->mDataChanged = false;
        emit this->dataChanged();
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
    nodeRoot.setAttribute("name", this->mDocumentName);

    QDomElement nodePicture = doc.createElement("picture");
    nodeRoot.appendChild(nodePicture);

    nodePicture.setAttribute("format", "png");

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    this->mContainer->image(0)->save(&buffer, "PNG");
    QString data = ba.toBase64();

    QDomText nodeData = doc.createTextNode(data);
    nodePicture.appendChild(nodeData);

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        doc.save(stream, 4);

        this->mFileName = fileName;
        this->mDataChanged = false;
        file.close();
        result = true;
        emit this->dataChanged();
    }
    return result;
}
//-----------------------------------------------------------------------------
bool EditorTabImage::changed()
{
    return this->mDataChanged;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::fileName()
{
    return this->mFileName;
}
//-----------------------------------------------------------------------------
QString EditorTabImage::documentName()
{
    return this->mDocumentName;
}
//-----------------------------------------------------------------------------
void EditorTabImage::setDocumentName(const QString &value)
{
    this->mDocumentName = value;
    emit this->dataChanged();
}
//-----------------------------------------------------------------------------
IDataContainer *EditorTabImage::dataContainer()
{
    return this->mContainer;
}
//-----------------------------------------------------------------------------
WidgetBitmapEditor *EditorTabImage::editor()
{
    return this->mEditor;
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
    <picture format="png">iVBORw0KGgoAAAANSUhEUgAAAAoAAAAKCAIAAAACUFjqAAAAA3NCSVQICAjb4U/gAAAACXBIWXMAAAsTAAALEwEAmpwYAAAARklEQVQYlYWPSQrAMAwDpZAX5x/Nl6eHgLMQt8IniRGWAeWqkux+zaD5my5B2z0uzHq0XegjW1+ZdLhbB4mkB0iHjY6fYS/sJjZR2Wu+lAAAAABJRU5ErkJggg==</picture>
</data>

 */
